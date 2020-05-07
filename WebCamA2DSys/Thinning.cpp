#include "Thinning.h"

CThinning::CThinning()
{
}

CThinning::~CThinning()
{
}

//×ü‰»ˆ—
int CThinning::Thinning(unsigned char * ucBinedImg, unsigned char * ucThinnedImage, long lWidth, long lHeight, long lIterativeLimit) {
	if (ucBinedImg == NULL)
		return -1;
	if (ucThinnedImage == NULL)
		return -2;
	if (lIterativeLimit == -1)
		lIterativeLimit = 60000;
	unsigned char x1, x2, x3, x4, x5, x6, x7, x8, xp;
	unsigned char g1, g2, g3, g4;
	unsigned char b1, b2, b3, b4;
	unsigned char np1, np2, npm;
	unsigned char *pUp, *pDown, *pImg;
	long    lDeletedPoints = 0;
	// set border 
	memcpy(ucThinnedImage, ucBinedImg, lWidth*lHeight);
	for (long it = 0; it<lIterativeLimit; it++) {
		lDeletedPoints = 0;
		for (long i = 1; i<lHeight - 1; i++) {
			// init neighborhood
			pUp = ucBinedImg + (i - 1)*lWidth;
			pImg = ucBinedImg + i*lWidth;
			pDown = ucBinedImg + (i + 1)*lWidth;

			for (long j = 1; j<lWidth - 1; j++) {
				pUp++;
				pImg++;
				pDown++;
				if (!*pImg)
					continue;
				x6 = *(pUp - 1);
				x5 = *(pImg - 1);
				x4 = *(pDown - 1);
				x7 = *pUp;
				xp = *pImg;
				x3 = *pDown;

				x8 = *(pUp + 1);
				x1 = *(pImg + 1);
				x2 = *(pDown + 1);

				b1 = !x1 && (x2 == 1 || x3 == 1);
				b2 = !x3 && (x4 == 1 || x5 == 1);
				b3 = !x5 && (x6 == 1 || x7 == 1);
				b4 = !x7 && (x8 == 1 || x1 == 1);

				g1 = (b1 + b2 + b3 + b4) == 1;

				np1 = x1 || x2;
				np1 += x3 || x4;
				np1 += x5 || x6;
				np1 += x7 || x8;
				np2 = x2 || x3;
				np2 += x4 || x5;
				np2 += x6 || x7;
				np2 += x8 || x1;

				npm = np1>np2 ? np2 : np1;
				g2 = npm >= 2 && npm <= 3;

				g3 = (x1 && (x2 || x3 || !x8)) == 0;
				g4 = (x5 && (x6 || x7 || !x4)) == 0;
				// first part
				if (g1 && g2 && g3) {
					// delete this point
					ucThinnedImage[lWidth*i + j] = 0;
					++lDeletedPoints;
				}
			}
		}
		//syn
		memcpy(ucBinedImg, ucThinnedImage, lWidth*lHeight);
		for (long i = 1; i<lHeight - 1; i++) {	// init neighborhood
			pUp = ucBinedImg + (i - 1)*lWidth;
			pImg = ucBinedImg + i*lWidth;
			pDown = ucBinedImg + (i + 1)*lWidth;
			for (long j = 1; j<lWidth - 1; j++) {
				pUp++;
				pImg++;
				pDown++;
				if (!*pImg)
					continue;
				x6 = *(pUp - 1);
				x5 = *(pImg - 1);
				x4 = *(pDown - 1);

				x7 = *pUp;
				xp = *pImg;
				x3 = *pDown;

				x8 = *(pUp + 1);
				x1 = *(pImg + 1);
				x2 = *(pDown + 1);

				b1 = !x1 && (x2 == 1 || x3 == 1);
				b2 = !x3 && (x4 == 1 || x5 == 1);
				b3 = !x5 && (x6 == 1 || x7 == 1);
				b4 = !x7 && (x8 == 1 || x1 == 1);

				g1 = (b1 + b2 + b3 + b4) == 1;

				np1 = x1 || x2;
				np1 += x3 || x4;
				np1 += x5 || x6;
				np1 += x7 || x8;
				np2 = x2 || x3;
				np2 += x4 || x5;
				np2 += x6 || x7;
				np2 += x8 || x1;

				npm = np1>np2 ? np2 : np1;
				g2 = npm >= 2 && npm <= 3;
				g3 = (x1 && (x2 || x3 || !x8)) == 0;
				g4 = (x5 && (x6 || x7 || !x4)) == 0;
				// second part
				if (g1 && g2 && g4) {	// delete this point
					ucThinnedImage[lWidth*i + j] = 0;
					++lDeletedPoints;
				}
			}
		}
		//syn
		memcpy(ucBinedImg, ucThinnedImage, lWidth*lHeight);
		// if no points to be deleted
		if (lDeletedPoints == 0)
			break;
	}
	// clear edge bar
	for (long i = 0; i<lHeight; i++) {
		for (long j = 0; j<lWidth; j++) {
			if (i<16)
				ucThinnedImage[i*lWidth + j] = 0;
			else if (i >= lHeight - 16)
				ucThinnedImage[i*lWidth + j] = 0;
			else if (j<16)
				ucThinnedImage[i*lWidth + j] = 0;
			else if (j >= lWidth - 16)
				ucThinnedImage[i*lWidth + j] = 0;
		}
	}
	return 0;
}

//×ü‰»‚ğ§Œä
void CThinning::Thinning(Mat& src, Mat& dst, long IterativeLimit/* = -1*/) {
	IterativeLimit = -1;
	Mat bin_img = src & 1;
	if (!dst.empty()) { dst.release(); }
	dst = Mat::zeros(src.size(), CV_8UC1);
	Thinning(bin_img.data, dst.data, bin_img.cols, bin_img.rows, IterativeLimit);
	dst *= 255;
}

//×ü‰»‚ğ§Œä
void CThinning::PlayThinning(Mat src, Mat& thinned) {
	//–c’£ˆ—
	//int dilete_size = 1;
	//Mat element = getStructuringElement(cv::MORPH_ELLIPSE,
	//	Size(2 * dilete_size + 1, 2 * dilete_size + 1),
	//	Point(dilete_size, dilete_size));
	//dilate(src, src, element);
	Thinning(src, thinned);
}

//‰æ‘f‚ğ–c’£‚³‚¹‚éˆ—@Œ»İ–¢g—p
void CThinning::Dilate(Mat s, Mat& d, int size) {
	Mat element = getStructuringElement(cv::MORPH_ELLIPSE,
		Size(2 * size + 1, 2 * size + 1),
		Point(size, size));
	dilate(s, d, element);
}
