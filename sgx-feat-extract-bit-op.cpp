#include <iostream>
#include <vector>
using namespace std;

int FindFeatureOffset(int nth_feature, int bitmap) {
	int count = 0;
	int sign_bit = 1;
	for (int i = 1; i <= sizeof(int)*8; i++) {
		sign_bit = ((i - nth_feature - 1) >> (sizeof(int)*8-1)) & 1;
		count += (bitmap & 1) & sign_bit;
		bitmap >>= 1;
	}
	return count;
}

float FetchFeature(const vector<float>& features, int nth_feature,
                   int bitmap) {
	int offset = FindFeatureOffset(nth_feature, bitmap);
	float ans = 0;
	for (int i = 1; i <= features.size(); i++) {
		ans += features[i-1] *
		           (i == offset && ((bitmap >> (nth_feature-1)) & 1));
	}
	return ans;
}

int main() {
	// your code goes here
	const int bitmap = 0b0101101001;
	const vector<float> features = {0.1, 0.2, 0.3, 0.4, 0.5};
	
	for (int i = 0; i < 10; i++) {
		cout << FetchFeature(features, i+1, bitmap) << endl;
	}
	return 0;
}
