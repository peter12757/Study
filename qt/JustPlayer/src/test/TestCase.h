#ifndef TESTCASE_H
#define TESTCASE_H

#include <string>




struct TestCase
{
    std::string filename;
    int video_width; //视频分辨率宽
    int video_height; //视频分辨率高
    TestCase(char *name,int width,int height) {}
};

const static TestCase case1 = TestCase((char*)"encode_yuv_1366_768_yuv444_Frame2.yuv",1366,768);
const static TestCase case2 = TestCase((char*)"case_716_1280_yuv_420.yuv",716,1280);

#endif // TESTCASE_H
