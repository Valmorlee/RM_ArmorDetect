//
// Created by valmorx on 25-2-14.
//

#include "tool.hpp"
#include <ArmorDetector.hpp>
#include "LightDescriptor.hpp"
#include <Parameter.hpp>
#include<bits/stdc++.h>
#include <opencv2/opencv.hpp>

#define BIG_ARMOR 1
#define SMALL_ARMOR 0
#define UNKNOWN_ARMOR -1

//Parameter param_tool;

void adjustRec(cv::RotatedRect& rect)
{
    float& width = rect.size.width;
    float& height = rect.size.height;
    float& angle = rect.angle;

    while(angle >= 90.0) angle -= 180.0;
    while(angle < -90.0) angle += 180.0;

    if(angle >= 45.0){
        std::swap(width, height);
        angle -= 90.0;
    }else if(angle < -45.0){
        std::swap(width, height);
        angle += 90.0;
    }

}

// 坐标两点间距离
double distance(const cv::Point2f &p1, const cv::Point2f &p2)
{
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}


void filterContours(std::vector<std::vector<cv::Point> > &contours, std::vector<LightDescriptor> &lightInfos, ArmorDetector &detector) {
    for (const auto & contour :contours) {
        double lightContoursArea = cv::contourArea(contour);

        //面积小于阈值过滤
        if (lightContoursArea < detector.param.light_min_area) continue;
        //椭圆拟合
        cv::RotatedRect rotated_rect = cv::fitEllipse(contour);
        //角度修正
        adjustRec(rotated_rect);
        //长宽比过大过滤
        if (rotated_rect.size.width / rotated_rect.size.height > detector.param.light_max_ratio) continue;
        //轮廓面积与椭圆面积比小于阈值过滤
        if (lightContoursArea / rotated_rect.size.area() < detector.param.light_contour_min_solidity) continue;
        //扩大识别区域
        rotated_rect.size.width *= detector.param.light_area_extend_ratio;
        rotated_rect.size.height *= detector.param.light_area_extend_ratio;

        lightInfos.emplace_back(LightDescriptor(rotated_rect));
    }
}


std::vector<ArmorDescriptor> filterArmors(std::vector<LightDescriptor> &lightInfos, ArmorDetector &detector) {
    std::vector<ArmorDescriptor> armors;

    std::sort(lightInfos.begin(), lightInfos.end(), [](const LightDescriptor &a, const LightDescriptor &b) {
        return a.center.x < b.center.x;
    });

    int pairs1 = 0,pairs2 = 0,pairs3 = 0;

    for (int i=0;i<lightInfos.size();i++) {
        for (int j=i+1;j<lightInfos.size();j++) {
            const LightDescriptor &leftlight = lightInfos[i];
            const LightDescriptor &rightlight = lightInfos[j];


            pairs1++;

            //角差
            double angleDiff = abs(leftlight.angle - rightlight.angle);
            //长度差比率
            double LenDiff_Ratio = abs(leftlight.length - rightlight.length) / std::max(leftlight.length, rightlight.length);

            if (angleDiff > detector.param.light_max_angle_diff ||
                LenDiff_Ratio > detector.param.light_max_angle_diff_ratio) continue;

            pairs2++;

            //左右灯条中心点间距离
            double dis = distance(leftlight.center, rightlight.center);
            //左右灯条长度平均值
            double avgLen = (leftlight.length + rightlight.length) / 2;
            //左右灯条中心y差值
            double yDiff = abs(leftlight.center.y - rightlight.center.y);
            //左右灯条中心x差值
            double xDiff = abs(leftlight.center.x - rightlight.center.x);
            //y差值比率
            double yDiff_Ratio = yDiff / avgLen;
            //x差值比率
            double xDiff_Ratio = xDiff / avgLen;

            //相距距离与灯条长度比值
            double dis_ratio = dis / avgLen;

            //筛选符合条件的灯条
             if (yDiff_Ratio > detector.param.light_max_y_diff_ratio ||
                 xDiff_Ratio < detector.param.light_min_x_diff_ratio ||
                 dis_ratio > detector.param.armor_max_aspect_ratio||
                 dis_ratio < detector.param.armor_min_aspect_ratio) {

                 continue;
             }

            pairs3++;

            //确认装甲板类型
            int armor_flag = dis_ratio > detector.param.armor_type_big_ratio ? BIG_ARMOR : SMALL_ARMOR;

            //Score
            double rotationScore = 100;

            // cv::Mat img = cv::Mat::ones(cv::Size(200,200),CV_8UC3);
            // cv::circle(img,leftlight.center,2,cv::Scalar(0,0,255),-1);
            // cv::circle(img,rightlight.center,2,cv::Scalar(0,0,255),-1);
            // cv::imshow("debug",img);
            // cv::waitKey(0);

            ArmorDescriptor armor(leftlight, rightlight,armor_flag,detector._grayImg,rotationScore,detector.param);

            armors.emplace_back(armor);
            break;
        }
    }

    /*
    std::cout << "初始遴选个数： " << pairs1 << std::endl
              << "条件2遴选后个数： " << pairs2 << std::endl
              << "条件3遴选后个数： " << pairs3 << std::endl;

    */
    return armors;
}

void drawArmor(ArmorDetector &detector,cv::Mat &img) {
    for (int i=0;i<detector._armors.size();i++) {
        std::vector<cv::Point2i> points;
        for (int j=0;j<4;j++) {
            points.emplace_back(cv::Point(static_cast<int>(detector._armors[i].vertex[j].x),
                                          static_cast<int>(detector._armors[i].vertex[j].y)));

        }

        cv::polylines(img, points, true, cv::Scalar(0, 255, 0), 2,8,0);
    }
    if (detector.is_debug) {
        cv::imshow("armors", img);
    }
}

cv::Point2f findIntersection(cv::Vec4f line1, cv::Vec4f line2) {
    // 提取直线的端点
    float x1 = line1[0], y1 = line1[1];
    float x2 = line1[2], y2 = line1[3];
    float x3 = line2[0], y3 = line2[1];
    float x4 = line2[2], y4 = line2[3];

    // 计算行列式的值
    float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    // 如果分母为0，说明直线平行或重合，没有唯一交点
    if (denominator == 0) {
        return cv::Point2f(-1, -1); // 返回一个无效点表示没有交点
    }

    // 计算交点坐标
    float px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denominator;
    float py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denominator;

    return cv::Point2f(px, py);
}

void drawPoint(const cv::Mat &img, cv::Point2f pt,const cv::Scalar &color) {
    cv::circle(img, pt, 5, color, -1);
}


void yolo_resizeUniform(const cv::Mat &src,cv::Mat &resized_img,cv::Size dstSize,object_rect &effect_roi, ArmorDetector &detector) {

    int w = src.cols;
    int h = src.rows;
    int dstW = dstSize.width;
    int dstH = dstSize.height;

    resized_img = cv::Mat(cv::Size(dstW, dstH), CV_8UC3, cv::Scalar(0, 0, 0));

    double ratio_src = w * 1.0 / h;
    double ratio_dst = dstW * 1.0 / dstH;

    int tmp_w = 0, tmp_h = 0;

    if (ratio_src > ratio_dst) {
        tmp_w = dstW;
        tmp_h = floor(1.0 * dstW / ratio_src);
    }else if(ratio_src < ratio_dst) {
        tmp_h = dstH;
        tmp_w = floor(1.0 * dstH * ratio_src);
    }else {
        cv::resize(src, resized_img, dstSize);
        effect_roi.x = 0;
        effect_roi.y = 0;
        effect_roi.width = dstW;
        effect_roi.height = dstH;

    }

    cv::Mat tmp;
    cv::resize(src, tmp, cv::Size(tmp_w, tmp_h));
    if (tmp_w != dstW)
    {
        int index_w = floor((dstW - tmp_w) / 2.0);
        for (int i = 0; i < dstH; i++)
        {
            memcpy(resized_img.data + i * dstW * 3 + index_w * 3, tmp.data + i * tmp_w * 3, tmp_w * 3);
            //像素重叠复制
        }
        effect_roi.x = index_w;
        effect_roi.y = 0;
        effect_roi.width = tmp_w;
        effect_roi.height = tmp_h;
    }
    else if (tmp_h != dstH)
    {
        int index_h = floor((dstH - tmp_h) / 2.0);

        memcpy(resized_img.data + index_h * dstW * 3, tmp.data, tmp_w * tmp_h * 3);
        effect_roi.x = 0;       // 实际图像区域在dst中x轴的相对坐标是 0
        effect_roi.y = index_h; // 实际图像区域在dst中y轴的相对坐标是 index_h
        effect_roi.width = tmp_w;
        effect_roi.height = tmp_h;
    }
    else
    {
        std::cout<<"error: resizeUniform"<<std::endl;
    }

    if (detector.is_debug) {
        detector._grayImg = resized_img;
        cv::imshow("resizeUniform", detector._grayImg);
    }

}

void yolo_normalize(cv::Mat &img, ArmorDetector &detector) {
    //处理
    img.convertTo(img,CV_32F);
    for (int i=0;i<img.rows;i++) {

        float *pdata = (float*)(img.data+i*img.step);
        for (int j=0;j<img.cols;j++) {
            pdata[0] = (pdata[0] - detector.param.mean[0]) / detector.param.std[0];
            pdata[1] = (pdata[1] - detector.param.mean[1]) / detector.param.std[1];
            pdata[2] = (pdata[2] - detector.param.mean[2]) / detector.param.std[2];
            pdata += 3;
        }

    }
    //is debug
    if (detector.is_debug) {
        cv::imshow("img_normalize", img);
    }
}

void yolo_getGridCenterX(std::vector<CenterX> &centers, ArmorDetector &detector) {
    for (int i=0;i<3;i++) {
        const int stride = detector.param.strides[i];
        int num_w = ceil(1.0*detector.param.onnx_width/stride);
        int num_h = ceil(1.0*detector.param.onnx_height/stride);

        for (int m=0;m<num_h;m++) {

            for (int n=0;n<num_w;n++) {
                CenterX center;
                center.stride = stride;
                center.x = n;
                center.y = m;
                centers.emplace_back(center);
            }

        }
    }
}

inline float fast_exp(float x) //unchecked
{
    union
    {
        uint32_t i;
        float f;
    } v{};
    v.i = (1 << 23) * (1.4426950409 * x + 126.93490512f);
    return v.f;
}

int yolo_activation_function_softmax(const float *src, float *dst, int length) //unchecked
{
    const float alpha = *std::max_element(src, src + length);
    float denominator{0};

    for (int i = 0; i < length; ++i)
    {
        dst[i] = fast_exp(src[i] - alpha);
        denominator += dst[i];
    }

    for (int i = 0; i < length; ++i)
    {
        dst[i] /= denominator;
    }

    return 0;
}

Info yolo_recPred2Box(const float* BoundBoxPre,int label_max_conf,float conf,const int x,const int y,const int stride,ArmorDetector &detector) {

    float center_x = x*stride;
    float center_y = y*stride;
    std::vector<float> recPred;
    recPred.resize(4);

    for (int i=0;i<4;i++) {
        float dis = 0;
        float *dis_after = new float [detector.param.reg_max+1];
        yolo_activation_function_softmax(BoundBoxPre+i*detector.param.reg_max,dis_after,detector.param.reg_max+1);

        for (int j=0;j<detector.param.reg_max+1;j++) {
            dis += dis_after[j]*j;
        }
        dis *= stride;
        recPred[i] = dis;
        delete [] dis_after;
    }

    float min_x = (std::max)(center_x - recPred[0], .0f);
    float min_y = (std::max)(center_y - recPred[1], .0f);
    float max_x = (std::min)(center_x + recPred[2], (float)detector.param.onnx_width);
    float max_y = (std::min)(center_y + recPred[3], (float)detector.param.onnx_height);

    //std::cout<<"min_x: "<<min_x<<" min_y: "<<min_y<<" max_x: "<<max_x<<" max_y: "<<max_y<<std::endl;

    return Info{min_x,min_y,max_x,max_y,conf,label_max_conf};

}

void yolo_decode2Box(cv::Mat &out,std::vector<CenterX> &centers,double threshold,std::vector<std::vector<Info>> &res_infos,ArmorDetector &detector) {
    const int PointX = centers.size(); // 2100个检测框

    //std::cout << PointX << std::endl;

    for (int i = 0; i < PointX; i++) {

        const int spec_x = centers[i].x;
        const int spec_y = centers[i].y;
        const int stride = centers[i].stride;

        // std::cout << spec_x << " " << spec_y << " " << stride << std::endl;

        double conf = 0;
        int label_max_conf = 0;

        // 遍历每个类别 find max conf 对应的index类别
        for (int index = 0; index < detector.param.num_class; index++) {

            double tmp = out.at<float>(0, i, index);
            if (tmp > conf) {
                conf = tmp;
                label_max_conf = index;
            }
        }

        //std::cout << conf << " " << label_max_conf << std::endl;
        if (conf > threshold) {

            const float *BoundBoxPre = out.ptr<float>(0, i) + detector.param.num_class;

            //std::cout<<BoundBoxPre[0]<<std::endl;

            auto tmp = yolo_recPred2Box(BoundBoxPre,label_max_conf,conf,spec_x,spec_y,stride,detector);
            res_infos[label_max_conf].emplace_back(tmp);
        }

    }

    //std::cout<<"> decode done"<<std::endl;
}

void NMS(std::vector<Info> &input_boxes, float NMS_THRESH)
{
    std::sort(input_boxes.begin(), input_boxes.end(), [](Info a, Info b)
              { return a.conf > b.conf; });
    std::vector<float> vArea(input_boxes.size());
    for (int i = 0; i < int(input_boxes.size()); ++i)
    {
        vArea[i] = (input_boxes.at(i).x2 - input_boxes.at(i).x1 + 1) * (input_boxes.at(i).y2 - input_boxes.at(i).y1 + 1);
    }
    for (int i = 0; i < int(input_boxes.size()); ++i)
    {
        for (int j = i + 1; j < int(input_boxes.size());)
        {
            float xx1 = (std::max)(input_boxes[i].x1, input_boxes[j].x1);
            float yy1 = (std::max)(input_boxes[i].y1, input_boxes[j].y1);
            float xx2 = (std::min)(input_boxes[i].x2, input_boxes[j].x2);
            float yy2 = (std::min)(input_boxes[i].y2, input_boxes[j].y2);
            float w = (std::max)(float(0), xx2 - xx1 + 1);
            float h = (std::max)(float(0), yy2 - yy1 + 1);
            float inter = w * h;
            float ovr = inter / (vArea[i] + vArea[j] - inter);
            if (ovr >= NMS_THRESH)
            {
                input_boxes.erase(input_boxes.begin() + j);
                vArea.erase(vArea.begin() + j);
            }
            else
            {
                j++;
            }
        }
    }
}

std::vector<Info> yolo_PreDetect(cv::Mat &resized_img,ArmorDetector &detector) {
    yolo_normalize(resized_img,detector);

    cv::Mat blob = cv::dnn::blobFromImage(resized_img);

    detector.myNet.setInput(blob);
    std::vector<cv::Mat> outs;
    detector.myNet.forward(outs, detector.myNet.getUnconnectedOutLayersNames());

    return yolo_PostDetect(outs[0],resized_img,detector);

}

std::vector<Info> yolo_PostDetect(cv::Mat &out, cv::Mat &resized_img, ArmorDetector &detector) {

    std::vector<CenterX> centers;
    yolo_getGridCenterX(centers, detector);

    // std::cout<<"centers size:"<<centers.size()<<std::endl; // 1 x 2100 x 41

    std::vector<std::vector<Info>> res;
    res.resize(centers.size()+5);

    yolo_decode2Box(out, centers, detector.param.conf_threshold, res, detector);

    std::vector<Info> res_infos;
    for (int i = 0 ; i < res.size(); i++) {
        NMS(res[i], detector.param.nms_threshold);
        for (auto box : res[i]) {
            res_infos.emplace_back(box);
        }
    }

    return res_infos;
}

cv::Point yolo_getCenterPoint(cv::Point pt1, cv::Point pt2, ArmorDetector &detector) {
    cv::Point centerPt;

    centerPt.x = (pt1.x + pt2.x) / 2;
    centerPt.y = (pt1.y + pt2.y) / 2;

    return centerPt;

}

cv::Mat yolo_draw_bboxes(const cv::Mat &bgr, std::vector<Info> &bboxes, object_rect &effect_roi,ArmorDetector &detector) //unchecked
{
    static const char *class_names[] = {"car_red", "car_blue", "car_unknow", "watcher_red",
                                        "watcher_blue", "watcher_unknow", "armor_red", "armor_blue", "armor_grey"};

    cv::Mat image = bgr.clone();

    int src_w = image.cols;
    int src_h = image.rows;
    int dst_w = effect_roi.width;
    int dst_h = effect_roi.height;
    float width_ratio = (float)src_w / (float)dst_w;
    float height_ratio = (float)src_h / (float)dst_h;

    for (size_t i = 0; i < bboxes.size(); i++)
    {
        Info &bbox = bboxes[i];

        if (detector.is_debug) {
            std::cout << "[" << bbox.x1 << " " << bbox.x2 << " " << bbox.y1 << " " << bbox.y2 << "] " << bbox.label << " " << 1.0*bbox.conf*100 << "%" << std::endl;
        }

        cv::Scalar color = cv::Scalar(detector.param.color_list[bbox.label][0], detector.param.color_list[bbox.label][1], detector.param.color_list[bbox.label][2]);


        if (detector.is_debug) cv::rectangle(detector._grayImg,cv::Rect(cv::Point(bbox.x1,bbox.y1),cv::Point(bbox.x2,bbox.y2)),color);

        cv::rectangle(image, cv::Rect(cv::Point((bbox.x1 - effect_roi.x) * width_ratio, (bbox.y1 - effect_roi.y) * height_ratio), cv::Point((bbox.x2 - effect_roi.x) * width_ratio, (bbox.y2 - effect_roi.y) * height_ratio)), color);

        cv::Point YOLO_Point = yolo_getCenterPoint(cv::Point((bbox.x1 - effect_roi.x) * width_ratio, (bbox.y1 - effect_roi.y) * height_ratio),cv::Point((bbox.x2 - effect_roi.x) * width_ratio, (bbox.y2 - effect_roi.y) * height_ratio),detector);
        detector.YOLO_Center_Points.emplace_back(YOLO_Point); //每轮要clear

        drawPoint(image,YOLO_Point,color);

        //std::cout<<"["<<(bbox.x1 - effect_roi.x) * width_ratio<<" "<<(bbox.x2 - effect_roi.x) * width_ratio<<" "<<(bbox.y1 - effect_roi.y) * height_ratio<<" "<<(bbox.y2 - effect_roi.y) * height_ratio<<"] "<<bbox.label<<" "<<1.0*bbox.conf*100<<"%"<<std::endl;

        char text[256];
        sprintf(text, "%s %.1f%%", class_names[bbox.label], bbox.conf * 100); //

        int baseLine = 0;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.4, 1, &baseLine);

        int x = (bbox.x1 - effect_roi.x) * width_ratio;
        int y = (bbox.y1 - effect_roi.y) * height_ratio - label_size.height - baseLine;
        if (y < 0)
            y = 0;
        if (x + label_size.width > image.cols)
            x = image.cols - label_size.width;

        cv::rectangle(image, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
                      color, -1);

        cv::putText(image, text, cv::Point(x, y + label_size.height),
                    cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
    }

    if (detector.is_debug) {
        cv::imshow("result_debug", detector._grayImg);
        for (auto pt:detector.YOLO_Center_Points) {
            std::cout << "PredictCenterPoint: " << pt <<std::endl;
        }
    }

    detector._displayImg = image;

    return image;
}
