#include "Opposition.h"

void Opposition(vector<unsigned char>& image, vector<unsigned char>& segmention) {
	//�����ط���
	for (int i = 0; i < image.size(); i++) {
		segmention[i] = 255 - image[i];
	}
}