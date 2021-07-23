
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    //open the video file for reading
    VideoCapture cap(0);

    // if not success, exit program
    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video file" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    //Defining window names
    String windowNameOriginalVideo = "Original Video";
    String windowNameBrightnessHigh50 = "Brightness Increased by 50";
    String windowNameWithBrightnessHigh100 = "Brightness Increased by 100";
    String windowNameBrightnessLow50 = "Brightness Decreased by 50";
    String windowNameBrightnessLow100 = "Brightness Decreased by 100";

    //Create and open windows for above window names
    namedWindow(windowNameOriginalVideo, WINDOW_FREERATIO);
    namedWindow(windowNameBrightnessHigh50, WINDOW_FREERATIO);
    namedWindow(windowNameWithBrightnessHigh100, WINDOW_FREERATIO);
    namedWindow(windowNameBrightnessLow50, WINDOW_FREERATIO);
    namedWindow(windowNameBrightnessLow100, WINDOW_FREERATIO);

    resizeWindow(windowNameOriginalVideo,16*20,9*20);
    resizeWindow(windowNameBrightnessHigh50,16*20,9*20);
    resizeWindow(windowNameWithBrightnessHigh100,16*20,9*20);
    resizeWindow(windowNameBrightnessLow50,16*20,9*20);
    resizeWindow(windowNameBrightnessLow100,16*20,9*20);

    while (true)
    {
        Mat frame;
        bool bSuccess = cap.read(frame); // read a new frame from video 

         //Breaking the while loop at the end of the video
        if (bSuccess == false)
        {
            cout << "Found the end of the video" << endl;
            break;
        }

        Mat frameBrighnessHigh50;
        frame.convertTo(frameBrighnessHigh50, -1, 1, 50); //increase the brightness by 50

        Mat frameBrighnessHigh100;
        frame.convertTo(frameBrighnessHigh100, -1, 1, 100); //increase the brightness by 100

        Mat frameBrighnessLow50;
        frame.convertTo(frameBrighnessLow50, -1, 1, -50); //decrease the brightness by 50

        Mat frameBrighnessLow100;
        frame.convertTo(frameBrighnessLow100, -1, 1, -100); //decrease the brightness by 100

        /* ============ Contrast Change ============

        Mat frameContrastHigh2;
        frame.convertTo(frameContrastHigh2, -1, 2, 0); //increase the contrast by 2

        Mat frameContrastHigh4;
        frame.convertTo(frameContrastHigh4, -1, 4, 0); //increase the contrast by 4

        Mat frameContrastLow0_5;
        frame.convertTo(frameContrastLow0_5, -1, 0.5, 0); //decrease the contrast by 0.5

        Mat frameContrastLow0_25;
        frame.convertTo(frameContrastLow0_25, -1, 0.25, 0); //decrease the contrast by 0.25

        =============================================== */    

        //Show above frames inside the created windows.
        imshow(windowNameOriginalVideo, frame);
        imshow(windowNameBrightnessHigh50, frameBrighnessHigh50);
        imshow(windowNameWithBrightnessHigh100, frameBrighnessHigh100);
        imshow(windowNameBrightnessLow50, frameBrighnessLow50);
        imshow(windowNameBrightnessLow100, frameBrighnessLow100);

        //wait for for 10 ms until any key is pressed.  
        //If the 'Esc' key is pressed, break the while loop.
        //If the any other key is pressed, continue the loop 
        //If any key is not pressed withing 10 ms, continue the loop
        if (waitKey(10) == 27)
        {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            break;
        }
    }

    return 0;
}