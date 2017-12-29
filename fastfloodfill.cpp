/* A fast flood fill algorithm
- based on stacked scan lines
- almost all filled pixels are visited just once
- optimized for MATLAB's matrix layout

MATLAB Calling Syntax

out [, cnt] = fastfloodfill(seedx, seedy, bw)

seedx: scalar
seedy: scalar
bw: H x W (logical)

out: H x W (logical)
cnt (optional): H x W (uint32) number of times each pixel visited

by Mengtian (Martin) Li, 2017
martinli.work@gmail.com

*/

#include <cstdint>
#include <cstdlib>
#include <array>
#include <stack>

#include <mex.h>
#include <tmwtypes.h>

typedef int64_t int_t;

class Point {
public:
	Point() = default;
	Point(int_t _x, int_t _y) : x(_x), y(_y) {}
	int_t x; int_t y;
};

template<typename T>
class Mat
{
public:
	Mat(void *ptr, size_t width, size_t height): data((T*)ptr), w(width), h(height)
	{
		if (!(ptr && width && height))
			mexErrMsgTxt("Invalid argument to init a Mat object");
	}

	inline T& operator () (size_t i) { return data[i]; }
	inline T& operator () (size_t x, size_t y) { return data[x*h+y]; }
	inline T& operator () (const Point &p) { return (*this)(p.x, p.y); }

	T *data;
	size_t w, h;
};

class floodfill
{/*
 A 4-way connectivity floodfill algorithm optimized for column-wise storage of boolean type
 Each filled pixel is visited exactly once
 Note that this is a class that works like a function
 Syntax: floodfill(seed, img);
 */

private:
	bool *data;
	uint32_t *cntdata;
	size_t colmax, rowmax, height, prevtop, prevbottom, top, bottom, col;
	typedef std::array<size_t, 3> stackele_t;
	std::stack<stackele_t> leftcols, rightcols;

	inline void push(std::stack<stackele_t> &s, size_t t, size_t b, size_t c)
	{
		s.emplace();
		auto & stacktop = s.top();
		stacktop[0] = t;
		stacktop[1] = b;
		stacktop[2] = c;
	}

	inline void pop(std::stack<stackele_t> &s)
	{
		auto & stacktop = s.top();
		prevtop = stacktop[0];
		prevbottom = stacktop[1];
		col = stacktop[2];
		s.pop();
	}

	bool fillcol()
	{ // return false means no more pixels can be filled (top & bottom are undefined)
		auto coldata = data + col*height;
		auto colcnt = cntdata + col*height;
		auto r = prevtop;
		if (coldata[r])
		{ // blocked, continue downward for opening
			colcnt[r]++;
			while (++r <= prevbottom && coldata[r]) colcnt[r]++;
			if (r > prevbottom)
				return false;
			colcnt[r]++;
			// now found the starting pixel (r)
			top = r;
			coldata[r] = true;
		}
		else
		{ // scan and fill upward
			colcnt[r]++;
			coldata[r] = true;
			while (r > 0 && !coldata[r - 1])
			{
				coldata[--r] = true;
				colcnt[r]++;
			}
			if (r > 0) colcnt[r - 1]++;
			top = r;
			r = prevtop;
		}
		// start downward
		while (r < rowmax && !coldata[r + 1])
		{
			coldata[++r] = true;
			colcnt[r]++;
		}
		if (r < rowmax) colcnt[r + 1]++;
		bottom = r;
		return true;
	}

	void fillleft()
	{
		while (1)
		{
			if (!fillcol()) return;
			if (prevtop > 1 && top < prevtop - 1) push(rightcols, top, prevtop - 2, col + 1);
			if (bottom > prevbottom + 1) push(rightcols, prevbottom + 2, bottom, col + 1);
			else if (prevbottom > 1 && bottom < prevbottom - 1) push(leftcols, bottom, prevbottom, col);
			if (!col) return;
			col--;
			prevtop = top;
			prevbottom = bottom;
		}
	}

	void fillright()
	{
		while (1)
		{
			if (!fillcol()) return;
			if (prevtop > 1 && top < prevtop - 1) push(leftcols, top, prevtop - 2, col - 1);
			if (bottom > prevbottom + 1) push(leftcols, prevbottom + 2, bottom, col - 1);
			else if (prevbottom > 1 && bottom < prevbottom - 1) push(rightcols, bottom + 2, prevbottom, col);
			if (col == colmax) return;
			col++;
			prevtop = top;
			prevbottom = bottom;
		}
	}

public:
	floodfill(Point seed, Mat<bool> &img, Mat<uint32_t> &cnt)
	{
		if (seed.x < 0 || seed.x >= img.w
			|| seed.y < 0 || seed.y >= img.h)
			return;

		colmax = img.w - 1;
		height = img.h;
		rowmax = height - 1;
		prevtop = prevbottom = seed.y;
		col = seed.x;
		data = img.data;
		cntdata = cnt.data;

		if(!fillcol()) return;
		prevtop = top;
		prevbottom = bottom;

		// start upward and save bottom for later
		if (col < colmax)
			push(rightcols, top, bottom, col + 1);
		if (col)
		{
			col--;
			fillleft();
		}
		while (1)
		{
			if (leftcols.size() > rightcols.size())
			{
				pop(leftcols);
				fillleft();
			}
			else
			{
				if (rightcols.empty())
					break;
				pop(rightcols);
				fillright();
			}
		}
	}
};

void mexFunction(int_t nlhs, mxArray *plhs[], int_t nrhs, const mxArray *prhs[])
{
	if (nrhs != 3) mexErrMsgTxt("Exactly 3 input arguments (seedx, seedy, bw) are expected");
	if (mxGetNumberOfElements(prhs[0]) != 1 || mxGetNumberOfElements(prhs[1]) != 1)
		mexErrMsgTxt("seedx and seedy must be scalars.");
	if (mxGetNumberOfDimensions(prhs[2]) != 2)
		mexErrMsgTxt("The input bw should be a single channel 2D matrix of logical type");
	int_t seedx = mxGetScalar(prhs[0]) - 1;
	int_t seedy = mxGetScalar(prhs[1]) - 1;
	size_t H = mxGetM(prhs[2]);
	size_t W = mxGetN(prhs[2]);
	Mat<bool> bw(mxGetData(prhs[2]), W, H);

	plhs[0] = mxCreateNumericMatrix(H, W, mxLOGICAL_CLASS, mxREAL); // it'll be init to 0
	Mat<bool> out(mxGetData(plhs[0]), W, H);
	memcpy(out.data, bw.data, W*H*sizeof(bool));

	auto mxcnt = mxCreateNumericMatrix(H, W, mxUINT32_CLASS, mxREAL); // it'll be init to 0
	Mat<uint32_t> cnt(mxGetData(mxcnt), W, H);
	if (nlhs > 1) plhs[1] = mxcnt;

	floodfill(Point(seedx, seedy), out, cnt);
	if (nlhs < 2) mxDestroyArray(mxcnt);
}

