// IA_MiniProject.cpp : Defines the entry point for the console application.
// Reference: http://www.aishack.in/tutorials/sudoku-grabber-with-opencv-plot/ for project pipeline and some arithmetic code segments


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/contrib/contrib.hpp"

#include <tesseract/baseapi.h>
#include <tesseract/strngs.h>

#include <iostream>
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <cstdlib>

#include <GlobalFunctions.h>
#include <Sudoku.h>

using namespace std;
using namespace cv;

int iterationCount;
double temperature;
double temperatureDropConstant;
int temperatureDropIterations;

void drawLine(Vec2f line, Mat &img, Scalar rgb = CV_RGB(255, 255, 255))
{
	if (line[1] != 0)
	{
		float m = -1 / tan(line[1]);

		float c = line[0] / sin(line[1]);

		cv::line(img, Point(0, c), Point(img.size().width, m*img.size().width + c), rgb);
	}
	else
	{
		cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
	}

}

void mergeRelatedLines(vector<Vec2f> *lines, Mat &img)
{
	vector<Vec2f>::iterator current;
	for (current = lines->begin(); current != lines->end(); current++)
	{
		if ((*current)[0] == 0 && (*current)[1] == -100) continue;
		float p1 = (*current)[0];
		float theta1 = (*current)[1];

		Point pt1current, pt2current;
		if (theta1>CV_PI * 45 / 180 && theta1<CV_PI * 135 / 180)
		{
			pt1current.x = 0;

			pt1current.y = p1 / sin(theta1);

			pt2current.x = img.size().width;
			pt2current.y = -pt2current.x / tan(theta1) + p1 / sin(theta1);
		}
		else
		{
			pt1current.y = 0;

			pt1current.x = p1 / cos(theta1);

			pt2current.y = img.size().height;
			pt2current.x = -pt2current.y / tan(theta1) + p1 / cos(theta1);

		}
		vector<Vec2f>::iterator    pos;
		for (pos = lines->begin(); pos != lines->end(); pos++)
		{
			if (*current == *pos) continue;
			if (fabs((*pos)[0] - (*current)[0])<20 && fabs((*pos)[1] - (*current)[1])<CV_PI * 10 / 180)
			{
				float p = (*pos)[0];
				float theta = (*pos)[1];
				Point pt1, pt2;
				if ((*pos)[1]>CV_PI * 45 / 180 && (*pos)[1]<CV_PI * 135 / 180)
				{
					pt1.x = 0;
					pt1.y = p / sin(theta);
					pt2.x = img.size().width;
					pt2.y = -pt2.x / tan(theta) + p / sin(theta);
				}
				else
				{
					pt1.y = 0;
					pt1.x = p / cos(theta);
					pt2.y = img.size().height;
					pt2.x = -pt2.y / tan(theta) + p / cos(theta);
				}
				if (((double)(pt1.x - pt1current.x)*(pt1.x - pt1current.x) + (pt1.y - pt1current.y)*(pt1.y - pt1current.y)<64 * 64) &&
					((double)(pt2.x - pt2current.x)*(pt2.x - pt2current.x) + (pt2.y - pt2current.y)*(pt2.y - pt2current.y)<64 * 64))
				{
					// Merge the two
					(*current)[0] = ((*current)[0] + (*pos)[0]) / 2;

					(*current)[1] = ((*current)[1] + (*pos)[1]) / 2;

					(*pos)[0] = 0;
					(*pos)[1] = -100;
				}
			}
		}
	}
}

char outputText[5];
void getOutputText(int value ,cv::Scalar& outputColor)
{
   snprintf(outputText, sizeof(outputText), "%d", value);
	switch (value)
	{
		case(1): outputColor = CV_RGB(255, 0, 0);
			break;
		case(2): outputColor = CV_RGB(0, 255, 0);
			break;
		case(3): outputColor = CV_RGB(0, 0, 255);
			break;
		case(4): outputColor = CV_RGB(255, 255, 0);
			break;
		case(5): outputColor = CV_RGB(255, 0, 255);
			break;
		case(6): outputColor = CV_RGB(0, 255, 255);
			break;
		case(7): outputColor = CV_RGB(50, 255, 50);
			break;
		case(8): outputColor = CV_RGB(50, 50, 50);
			break;
		case(9): outputColor = CV_RGB(25, 100, 25);
			break;
		default: outputColor = CV_RGB(0, 0, 0);
	}
}

int main(int argc, char** argv)
{


	tesseract::TessBaseAPI tess;
	tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
	tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);

	Mat characters[9];
	{
		characters[0] = imread("1.jpg", 0);
		characters[1] = imread("2.jpg", 0);
		characters[2] = imread("3.jpg", 0);
		characters[3] = imread("4.jpg", 0);
		characters[4] = imread("5.jpg", 0);
		characters[5] = imread("6.jpg", 0);
		characters[6] = imread("7.jpg", 0);
		characters[7] = imread("8.jpg", 0);
		characters[8] = imread("9.jpg", 0);
	}



	namedWindow("Original", 1);
	//namedWindow("Processed", 1);

	Mat sudoku = imread(argv[1], 0);
	imshow("Original", sudoku);
	waitKey(0);

	//defining outerbox
	Mat outerBox = Mat(sudoku.size(), CV_8UC1);

	//coping original
	Mat original = sudoku.clone();

	//Preprocessing the image
	GaussianBlur(sudoku, sudoku, Size(11, 11), 0);
	//imshow("Original", sudoku);
	//waitKey(0);

	//thresholding the image
	adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
	//imshow("Processed", outerBox);
	//waitKey(0);

	//reverting the image
	bitwise_not(outerBox, outerBox);
	//imshow("Processed", outerBox);
	//waitKey(0);

	//dilating to fill gaps in connected parts
	Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	dilate(outerBox, outerBox, kernel);
	//imshow("Processed", outerBox);
	//waitKey(0);


	//finding biggest blob
	int count = 0;
	int max = -1;

	Point maxPt;

	for (int y = 0; y<outerBox.size().height; y++)
	{
		uchar *row = outerBox.ptr(y);
		for (int x = 0; x<outerBox.size().width; x++)
		{
			if (row[x] >= 128)
			{

				int area = floodFill(outerBox, Point(x, y), CV_RGB(0, 0, 64));

				if (area>max)
				{
					maxPt = Point(x, y);
					max = area;
				}
			}
		}

	}

	floodFill(outerBox, maxPt, CV_RGB(255, 255, 255));
	//imshow("Processed", outerBox);
	//waitKey(0);

	//
	for (int y = 0; y<outerBox.size().height; y++)
	{
		uchar *row = outerBox.ptr(y);
		for (int x = 0; x<outerBox.size().width; x++)
		{
			if (row[x] == 64 && x != maxPt.x && y != maxPt.y)
			{
				int area = floodFill(outerBox, Point(x, y), CV_RGB(0, 0, 0));
			}
		}
	}

	//imshow("Processed", outerBox);
	//waitKey(0);

	erode(outerBox, outerBox, kernel);
	//imshow("Processed", outerBox);
	//waitKey(0);

	Mat faded = outerBox / 2;

	//original += faded;

	vector<Vec2f> lines;
	HoughLines(outerBox, lines, 1, CV_PI / 180, 200);
	//imshow("Processed", outerBox);
	//waitKey(0);

	for (int i = 0; i<lines.size(); i++)
	{
		drawLine(lines[i], outerBox, CV_RGB(0, 0, 128));
	}

	//imshow("Processed", outerBox);
	//waitKey(0);

	mergeRelatedLines(&lines, sudoku); // Add this line
	//imshow("Processed", outerBox);
	//waitKey(0);

	// Now detect the lines on extremes
	Vec2f topEdge = Vec2f(1000, 1000);    double topYIntercept = 100000, topXIntercept = 0;
	Vec2f bottomEdge = Vec2f(-1000, -1000);        double bottomYIntercept = 0, bottomXIntercept = 0;
	Vec2f leftEdge = Vec2f(1000, 1000);    double leftXIntercept = 100000, leftYIntercept = 0;
	Vec2f rightEdge = Vec2f(-1000, -1000);        double rightXIntercept = 0, rightYIntercept = 0;

	for (int i = 0; i<lines.size(); i++)
	{

		Vec2f current = lines[i];

		float p = current[0];

		float theta = current[1];

		if (p == 0 && theta == -100)
			continue;
		double xIntercept, yIntercept;
		xIntercept = p / cos(theta);
		yIntercept = p / (cos(theta)*sin(theta));
		if (theta>CV_PI * 80 / 180 && theta<CV_PI * 100 / 180)
		{
			if (p<topEdge[0])

				topEdge = current;

			if (p>bottomEdge[0])
				bottomEdge = current;
		}
		else if (theta<CV_PI * 10 / 180 || theta>CV_PI * 170 / 180)
		{
			if (xIntercept>rightXIntercept)
			{
				rightEdge = current;
				rightXIntercept = xIntercept;
			}
			else if (xIntercept <= leftXIntercept)
			{
				leftEdge = current;
				leftXIntercept = xIntercept;
			}
		}
	}
	drawLine(topEdge, sudoku, CV_RGB(0, 0, 0));
	drawLine(bottomEdge, sudoku, CV_RGB(0, 0, 0));
	drawLine(leftEdge, sudoku, CV_RGB(0, 0, 0));
	drawLine(rightEdge, sudoku, CV_RGB(0, 0, 0));

	Point left1, left2, right1, right2, bottom1, bottom2, top1, top2;

	int height = outerBox.size().height;

	int width = outerBox.size().width;

	if (leftEdge[1] != 0)
	{
		left1.x = 0;        left1.y = leftEdge[0] / sin(leftEdge[1]);
		left2.x = width;    left2.y = -left2.x / tan(leftEdge[1]) + left1.y;
	}
	else
	{
		left1.y = 0;        left1.x = leftEdge[0] / cos(leftEdge[1]);
		left2.y = height;    left2.x = left1.x - height*tan(leftEdge[1]);

	}

	if (rightEdge[1] != 0)
	{
		right1.x = 0;        right1.y = rightEdge[0] / sin(rightEdge[1]);
		right2.x = width;    right2.y = -right2.x / tan(rightEdge[1]) + right1.y;
	}
	else
	{
		right1.y = 0;        right1.x = rightEdge[0] / cos(rightEdge[1]);
		right2.y = height;    right2.x = right1.x - height*tan(rightEdge[1]);

	}

	bottom1.x = 0;    bottom1.y = bottomEdge[0] / sin(bottomEdge[1]);

	bottom2.x = width; bottom2.y = -bottom2.x / tan(bottomEdge[1]) + bottom1.y;

	top1.x = 0;        top1.y = topEdge[0] / sin(topEdge[1]);
	top2.x = width;    top2.y = -top2.x / tan(topEdge[1]) + top1.y;

	// Next, we find the intersection of  these four lines
	double leftA = left2.y - left1.y;
	double leftB = left1.x - left2.x;

	double leftC = leftA*left1.x + leftB*left1.y;

	double rightA = right2.y - right1.y;
	double rightB = right1.x - right2.x;

	double rightC = rightA*right1.x + rightB*right1.y;

	double topA = top2.y - top1.y;
	double topB = top1.x - top2.x;

	double topC = topA*top1.x + topB*top1.y;

	double bottomA = bottom2.y - bottom1.y;
	double bottomB = bottom1.x - bottom2.x;

	double bottomC = bottomA*bottom1.x + bottomB*bottom1.y;

	// Intersection of left and top
	double detTopLeft = leftA*topB - leftB*topA;

	CvPoint ptTopLeft = cvPoint((topB*leftC - leftB*topC) / detTopLeft, (leftA*topC - topA*leftC) / detTopLeft);

	// Intersection of top and right
	double detTopRight = rightA*topB - rightB*topA;

	CvPoint ptTopRight = cvPoint((topB*rightC - rightB*topC) / detTopRight, (rightA*topC - topA*rightC) / detTopRight);

	// Intersection of right and bottom
	double detBottomRight = rightA*bottomB - rightB*bottomA;
	CvPoint ptBottomRight = cvPoint((bottomB*rightC - rightB*bottomC) / detBottomRight, (rightA*bottomC - bottomA*rightC) / detBottomRight);// Intersection of bottom and left
	double detBottomLeft = leftA*bottomB - leftB*bottomA;
	CvPoint ptBottomLeft = cvPoint((bottomB*leftC - leftB*bottomC) / detBottomLeft, (leftA*bottomC - bottomA*leftC) / detBottomLeft);

	int maxLength = (ptBottomLeft.x - ptBottomRight.x)*(ptBottomLeft.x - ptBottomRight.x) + (ptBottomLeft.y - ptBottomRight.y)*(ptBottomLeft.y - ptBottomRight.y);
	int temp = (ptTopRight.x - ptBottomRight.x)*(ptTopRight.x - ptBottomRight.x) + (ptTopRight.y - ptBottomRight.y)*(ptTopRight.y - ptBottomRight.y);

	if (temp>maxLength) maxLength = temp;

	temp = (ptTopRight.x - ptTopLeft.x)*(ptTopRight.x - ptTopLeft.x) + (ptTopRight.y - ptTopLeft.y)*(ptTopRight.y - ptTopLeft.y);

	if (temp>maxLength) maxLength = temp;

	temp = (ptBottomLeft.x - ptTopLeft.x)*(ptBottomLeft.x - ptTopLeft.x) + (ptBottomLeft.y - ptTopLeft.y)*(ptBottomLeft.y - ptTopLeft.y);

	if (temp>maxLength) maxLength = temp;

	maxLength = sqrt((double)maxLength);

	Point2f src[4], dst[4];
	src[0] = ptTopLeft;            dst[0] = Point2f(0, 0);
	src[1] = ptTopRight;        dst[1] = Point2f(maxLength - 1, 0);
	src[2] = ptBottomRight;        dst[2] = Point2f(maxLength - 1, maxLength - 1);
	src[3] = ptBottomLeft;        dst[3] = Point2f(0, maxLength - 1);

	//imshow("Original", original);
	//waitKey(0);

	Mat undistorted = Mat(Size(maxLength, maxLength), CV_8UC1);
	cv::warpPerspective(original, undistorted, cv::getPerspectiveTransform(src, dst), Size(maxLength, maxLength));
	cv::warpPerspective(original, original, cv::getPerspectiveTransform(src, dst), Size(maxLength, maxLength));

	double scale = undistorted.rows / 500.0;
	//Mat undistorted2;
	resize(undistorted, undistorted, Size((int)undistorted.rows / scale, (int)undistorted.cols / scale));
	resize(original, original, Size((int)undistorted.rows / scale, (int)undistorted.cols / scale));

	//imshow("Processed", undistorted);
	//waitKey(0);
//    Mat finalSolved = original;
//    imshow("Originaler", finalSolved);
//	waitKey(0);


	Mat undistortedThreshed = undistorted.clone();
	adaptiveThreshold(undistorted, undistortedThreshed, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 101, 1);

	//imshow("Original", undistortedThreshed);
	//waitKey(0);

	outerBox = undistortedThreshed.clone();

	//finding biggest blob
	 count = 0;
	 max = -1;



	for (int y = 0; y<outerBox.size().height; y++)
	{
		uchar *row = outerBox.ptr(y);
		for (int x = 0; x<outerBox.size().width; x++)
		{
			if (row[x] >= 128)
			{

				int area = floodFill(outerBox, Point(x, y), CV_RGB(0, 0, 64));

				if (area>max)
				{
					maxPt = Point(x, y);
					max = area;
				}
			}
		}

	}
	floodFill(outerBox, maxPt, CV_RGB(255, 255, 255));
	//imshow("Processed", outerBox);
	//waitKey(0);

	undistortedThreshed -= outerBox;
	//imshow("Original", undistortedThreshed);
	//waitKey(0);

	threshold(undistortedThreshed, undistortedThreshed, 100, 255, THRESH_BINARY);
	//dilate(undistortedThreshed, undistortedThreshed, kernel);
	//adaptiveThreshold(undistortedThreshed, undistortedThreshed, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
	imshow("Original", undistortedThreshed);
	waitKey(0);

	maxLength = undistortedThreshed.rows;

	int dist = ceil((double)maxLength / 9);
	Mat currentCell = Mat(dist, dist, CV_8UC1);


    Cell sudokus[9][9];

	for (int j = 0; j<9; j++)
	{
		for (int i = 0; i<9; i++)
		{
			for (int y = 0; y<dist && j*dist + y<undistortedThreshed.cols; y++)
			{

				uchar* ptr = currentCell.ptr(y);

				for (int x = 0; x<dist && i*dist + x<undistortedThreshed.rows; x++)
				{
					ptr[x] = undistortedThreshed.at<uchar>(j*dist + y, i*dist + x);
				}
			}
			Moments m = cv::moments(currentCell, true);
			//imshow("Original",currentCell);
			//imwrite("temp.jpg", currentCell);
			//waitKey(00);
			int area = m.m00;
			//if (area > currentCell.rows*currentCell.cols / 5)
			//{
			//	int number = dr->classify(currentCell);
			//	printf("%d ", number);
			//}
			//else
			//{
			//	printf("  ");
			//}
			//erode(currentCell, currentCell, kernel);
			tess.SetImage((uchar*)currentCell.data, currentCell.cols, currentCell.rows, 1, currentCell.cols);

			// Get the text
			char* out = tess.GetUTF8Text();
			if (out[0] == '\0')
			{
				//cout << "0 ";
                sudokus[j][i].value = 0;
                sudokus[j][i].isFixed = false;
			}
			else if (!isdigit(out[0]))
			{
				//cout << "0 ";
                sudokus[j][i].value = 0;
                sudokus[j][i].isFixed = false;
            }
			else
			{
				//std::cout << out[0] << " ";
                sudokus[j][i].value = atoi(out);
                sudokus[j][i].isFixed = true;
			}


		}

		printf("\n");
	}
	//destroyAllWindows();



	Sudoku sudokuA(sudokus);
	iterationCount = 0;
	temperature = 0.05;
	temperatureDropConstant = 0.9995;
	temperatureDropIterations = 4000;
	unsigned int seed = time(NULL);
	SimulationType  simType;


	cout<<"Sudoku (from input file)"<<endl;
	sudokuA.PrintSudoku();
	sudokuA.calculateFitness();
	cout<<"Fitness = " << sudokuA.GetFitness() << endl << endl << endl;

	cout<<"Sudoku (Initialized)"<<endl;
	sudokuA.fillSudoku();
	sudokuA.PrintSudoku();
	sudokuA.calculateFitness();
	cout<<"Fitness = " << sudokuA.GetFitness() << endl << endl << endl;

    SimulatedAnnealing(sudokuA,seed);

    Location loc;
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scaleI = 1;
    int thickness = 2;
    int baseline = 0;

   // char outputText[5];

    Mat colorOutput;

    if (original.type()==CV_8UC1) {
              //input image is grayscale
    cvtColor(original, colorOutput, CV_GRAY2RGB);

  } else {
    colorOutput = original;
  }

  cv::Scalar outputColor = CV_RGB(255,0,0);

	for (int j = 0; j<9; j++)
	{
		for (int i = 0; i<9; i++)
		{
            loc.row = j;
            loc.col = i;
			if (sudokuA.isFixed(loc) ) continue;

			cv::Rect roi(cv::Point((i * undistortedThreshed.cols / 9)+4, (j * undistortedThreshed.rows / 9)+4), characters[sudokuA.getValue(loc)  - 1].size());
			cv::Rect roi2(cv::Point((i * original.cols / 9) + 4, (j * original.rows / 9) + 4), characters[sudokuA.getValue(loc)  - 1].size());

			//resize(characters[sudokuA.getValue(loc) - 1], characters[sudokuA.getValue(loc) - 1], Size(undistortedThreshed.cols/9, undistortedThreshed.rows/9));
			characters[sudokuA.getValue(loc) -1].copyTo(undistortedThreshed(roi));

			addWeighted(characters[sudokuA.getValue(loc)  - 1], -1, original(roi2), 1, 0.0, original(roi2));
            //writing on original image
            getOutputText(sudokuA.getValue(loc) ,outputColor);
              cv::putText(colorOutput, outputText, cv::Point((i * original.cols / 9) + (original.cols / 18), (j * original.rows / 9) + (original.rows / 18)), fontface, scaleI, outputColor, thickness, 8);

			imshow("Processed", undistortedThreshed);
			waitKey(200);
		}

	}

	undistortedThreshed += outerBox;

	//imshow("Original", undistortedThreshed);
	//imwrite("solved.jpg", undistortedThreshed);
	//waitKey(0);

	imshow("Original", original);
	//imwrite("final.jpg", original);

	imshow("Color", colorOutput);
	//imwrite("final.jpg", original);
	waitKey(0);
	return 0;
}

