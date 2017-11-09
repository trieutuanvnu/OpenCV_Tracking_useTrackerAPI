
#include <opencv2/core/core.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <string>
#include <math.h>

using namespace cv;
using namespace std;

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

int main(int argc, char **argv)
{

	// List of tracker types in OpenCV 3.2
	// NOTE : GOTURN implementation is buggy and does not work.
	string trackerTypes[5] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW" };
	// vector <string> trackerTypes(types, std::end(types));

	// Create a tracker
	string trackerType = trackerTypes[0]; // you can select the tracking type here. 

	Ptr<Tracker> tracker;

	if (trackerType == "BOOSTING")
		tracker = TrackerBoosting::create();
	if (trackerType == "MIL")
		tracker = TrackerMIL::create();
	if (trackerType == "KCF")
		tracker = TrackerKCF::create();
	if (trackerType == "TLD")
		tracker = TrackerTLD::create();
	if (trackerType == "TLD")
		tracker = TrackerMedianFlow::create();

	// Read video
	VideoCapture video("fb.mp4");
	Mat frame;
	Mat resizeframe;

	// Exit if video is not opened
	if(!video.isOpened())
	{
		cout << "Could not read video file" << endl;
		return 1;

	}

	// Read first frame

	bool ok = video.read(frame);
	resize(frame, resizeframe, Size(800,480));
	namedWindow("avc", 0);
	// Define initial boundibg box
	Rect2d bbox(287, 23, 86, 320);

	// Uncomment the line below to select a different bounding box
	bbox = selectROI(resizeframe, false);

	// Display bounding box.
	rectangle(resizeframe, bbox, Scalar(255, 0, 0), 2, 1);
	imshow("Tracking", resizeframe);

	tracker->init(resizeframe, bbox);

//	tracker->update(resizeframe, bbox);
	while (video.read(frame))
	{
		
		resize(frame, resizeframe, Size(800, 480));
		// Start timer
		double timer = (double)getTickCount();

		// Update the tracking result
		bool ok = tracker->update(resizeframe, bbox);

		// Calculate Frames per second (FPS)
		float fps = getTickFrequency() / ((double)getTickCount() - timer);

		if (ok)
		{
			// Tracking success : Draw the tracked object
			rectangle(resizeframe, bbox, Scalar(255, 0, 0), 2, 1);
		}
		else
		{
			// Tracking failure detected.
			putText(resizeframe, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
		}

		// Display tracker type on frame
		putText(resizeframe, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

		// Display FPS on frame
		putText(resizeframe, "FPS : " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

		// Display frame.
		imshow("Tracking", resizeframe);

		// Exit if ESC pressed.
		int k = waitKey(1);
		if (k == 27)
		{
			break;
		}

	}
}