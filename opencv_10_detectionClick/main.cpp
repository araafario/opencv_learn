#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

bool enable,clicked;
Mat imgOriginal;
int iLowH = 0;
int iHighH = 179;

int iLowS = 0;
int iHighS = 255;

int iLowV = 0;
int iHighV = 255;

String control = "Control";
String original = "Original";
String thresholded = "threshold";

void eventHandler(int event, int x, int y, int flags, void* param){
	if(event == EVENT_RBUTTONDOWN && clicked == false){
        clicked = true;
		Mat image = imgOriginal.clone();
        Vec3b rgb = image.at < Vec3b > (y, x);
        int B = rgb.val[0];
        int G = rgb.val[1];
        int R = rgb.val[2];

        Mat HSV;
        Mat RGB = image(Rect(x, y, 1, 1));
        cvtColor(RGB, HSV, COLOR_BGR2HSV);

        Vec3b hsv = HSV.at < Vec3b > (0, 0);
        iLowH = hsv.val[0];
        iLowS = hsv.val[1];
        iLowV = hsv.val[2];
        cout << iLowH << " " << iLowS << " " << iLowV << endl;
	}
    if(event == EVENT_RBUTTONUP && clicked == true){
        clicked = false;
    }
}

void trackBar(){
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 179);

    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);
}

void trackBar_update(){	
    setTrackbarPos("LowH", "Control", iLowH);
	setTrackbarPos("HighH", "Control", iHighH);
	
    setTrackbarPos("LowS", "Control", iLowS);
    setTrackbarPos("HighS", "Control", iHighS);
	
    setTrackbarPos("LowV", "Control", iLowV);
	setTrackbarPos("HighV", "Control", iHighV);
}

int main(int argc, char ** argv) {
    VideoCapture cap(0); //capture the video from webcam

    if (!cap.isOpened()) // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    enable = clicked = false;

    namedWindow(control, WINDOW_NORMAL); //create a window called "Control"
    namedWindow(original, WINDOW_NORMAL); //create a window called "Control"
    namedWindow(thresholded, WINDOW_NORMAL); //create a window called "Control"
    resizeWindow(control,200,100);
    resizeWindow(original,1280/2,720/2);
    resizeWindow(thresholded,1280/2,720/2);
    
    //Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 179);

    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);

    setMouseCallback(original, eventHandler);

    int iLastX = -1;
    int iLastY = -1;

    //Capture a temporary image from the camera
    Mat imgTmp;
    cap.read(imgTmp);

    //Create a black image with the size as the camera output
    Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;

    while (true) {

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

        //morphological opening (removes small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //morphological closing (removes small holes from the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //Calculate the moments of the thresholded image
        Moments oMoments = moments(imgThresholded);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00;

        // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
        if (dArea > 10000) {
            //calculate the position of the ball
            int posX = dM10 / dArea;
            int posY = dM01 / dArea;

            if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0) {
                //Draw a red line from the previous point to the current point
                if (enable == true) line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 5);
            }

            iLastX = posX;
            iLastY = posY;
        }

        Mat flipTresholded;
        flip(imgThresholded,flipTresholded,1);
        imshow(thresholded, flipTresholded); //show the thresholded image

        imgOriginal = imgOriginal + imgLines;
        
        Mat flipOriginal;
        flip(imgOriginal,flipOriginal,1);

        imshow(original, flipOriginal); //show the original image

        trackBar();
        trackBar_update();

        if (waitKey(1) == 122) enable ^= true;

        if (waitKey(1) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;
}