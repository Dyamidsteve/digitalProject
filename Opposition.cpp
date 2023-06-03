#include "Opposition.h"

void Opposition(vector<unsigned char>& image, vector<unsigned char>& segmention) {
	//½«ÏñËØ·´Ïà
	for (int i = 0; i < image.size(); i++) {
		segmention[i] = 255 - image[i];
	}
}
