#include "ImageHandler.h"
#include <iostream>
int main()
{
	int menu_id;
	while (true)
	{
		printf("Simple Image Processing Software\n");
		printf("menu : ");
		scanf("%d", &menu_id);
		switch (menu_id)
		{
		case 1:
		{
			printf("file open\n");
			printf("filename : ");
			string filename;
			cin >> filename;
			if(open(filename)) printf("succeed\n");
			else printf("fail\n");
			break;
		}
		case 2: 
			printf("sava\n");
			save();
			printf("succeed\n");
			break;
		case 3: 
		{
			printf("save ax\n");
			printf("filename : ");
			string filename;
			cin >> filename;
			savs_as(filename);
			printf("succeed!\n");
			break;
		}
		case 4: 
			printf("undo\n");
			if (undo()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 5: 
			printf("rgb to gray\n");
			if(rgb_to_gray()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 6: 
			printf("inverse vertical\n");
			if (inverse_vertical()) printf("succeed\n");
			else printf("fail\n");
			break; 
		case 7: 
			printf("inverse horizontal\n");
			if (inverse_horizontal()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 8:
		{
			int num = -1;
			printf("bit plane\n");
			while (num < 0 || num > 7)
			{
				printf("input number between 0 and 7 : ");
				scanf("%d", &num);
			}
			if (show_bit_plane(num)) printf("succeed\n");
			else printf("fail\n");
			break;
		}
		case 9:
			printf("histogram equalization\n");
			if (histogram_equalization()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 10: 
			printf("histogram stretch\n");
			if (stretch()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 11: 
		{
			printf("histogram stretch\n");
			int max = 256, min = -1;
			while (max > 255 || max <= min || min < 0)
			{
				printf("input min and max level in order: ");
				scanf("%d %d", &min, &max);
			}
			if (stretch(min, max)) printf("succeed\n");
			else printf("fail\n");
			break;
		}
		case 12: 
			printf("negative\n");
			if (negative()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 13: 
		{
			printf("thresholding\n");
			int t = 256;
			while (t > 255 || t < 0)
			{
				printf("threshold : ");
				scanf("%d", &t);
			}
			if (thresholding(t)) printf("succeed\n");
			else printf("fail\n");
			break;
		}
		case 14: 
			printf("gaussian blur 3¡¿3\n");
			if (gaussian_blur(3)) printf("succeed\n");
			else printf("fail\n");
			break;
		case 15: 
			printf("gaussian blur 5¡¿5\n");
			if (gaussian_blur(5)) printf("succeed\n");
			else printf("fail\n");
			break;
		case 16: 
			printf("unsharp\n");
			if (unsharp()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 17: 
			printf("average filter\n");
			if (average_filter()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 18: 
			printf("median filter\n");
			if (median_filter()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 19: 
			printf("sobel masking\n");
			if (sobel_masking()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 20: 
			printf("laplacian masking\n");
			if (laplacian_masking()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 21: 
			printf("opning\n");
			if (mopology_opne()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 22: 
			printf("closing\n");
			if (mopology_close()) printf("succeed\n");
			else printf("fail\n");
			break;
		case 23: 
		{
			printf("expansion\n");
			double r = -1;
			while (r < 1)
			{
				printf("input expansion ratio : ");
				scanf("%lf", &r);
			}
			int w = get_width() * r;
			int h = get_height() * r;
			if (resize(w, h)) printf("succeed\n");
			else printf("fail\n");
			break;
		}
		case 24:
		{
			printf("reduction\n");
			double r = -1;
			while (r < 1)
			{
				printf("input reduction ratio : ");
				scanf("%lf", &r);
			}
			int w = get_width() / r;
			int h = get_height() / r;
			if (resize(w, h)) printf("succeed\n");
			else printf("fail\n");
			break;
		}
		case 25:
		{
			printf("expansion with interpolation\n");
			double r = -1;
			while (r < 1)
			{
				printf("input expansion ratio : ");
				scanf("%lf", &r);
			}
			int w = get_width() * r;
			int h = get_height() * r;
			if (expansion_interpolation(w, h)) printf("succeed\n");
			else printf("fail\n");
			break;
		}
		case 26:
		{
			printf("reduction with interpolation\n");
			double r = -1;
			while (r < 1)
			{
				printf("input reduction ratio : ");
				scanf("%lf", &r);
			}
			int w = get_width() / r;
			int h = get_height() / r;
			if (reduce_interpolation(w, h)) printf("succeed\n");
			else printf("fail\n");
			break;
		}
		case 27:
		{
			printf("rotation\n");
			printf("input degrees : ");
			int d;
			scanf("%d", &d);
			if (rotation(d)) printf("succeed\n");
			else printf("fail\n");
			break;
		}
		case -999: return 0;
		default:
			show();
			break;
		}
		printf("\n");
	}
	return 0;
}