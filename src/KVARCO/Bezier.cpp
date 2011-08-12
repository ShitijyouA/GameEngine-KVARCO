#include "pch.h"
#include "Bezier.h"

Bezier::Bezier(xtal::ArrayPtr points,float speed,DWORD max_)
{
	const DWORD	DIVIDE=static_cast<DWORD>(ceil(max_/speed));
	const WORD	Dimension=3;

	std::vector<float> mPs;
	const float InvU=1.0f/(DIVIDE-1);
	for(DWORD i=0; i<DIVIDE; i++)
	{
		float u=i*InvU;
		for(WORD ii=0; ii<Dimension; ii++)
		{
			float mP=
				3*((ii==0 || ii==Dimension) ? 1:0)*pow(u,ii)*pow(1-u,Dimension-ii);
			mPs.push_back(mP);
		}

		fPointPtrX ptr=xtal::xnew<fPoint>();
		for(WORD ii=0; ii<Dimension; ii++)
		{
			fPointPtrX sp=xtal::ptr_cast<fPoint>(points->at(ii));
			if(xtal::is_null(sp)) continue;
			ptr->x+=sp->x*mPs[ii];
			ptr->y+=sp->y*mPs[ii];
		}
		Points.push_back(ptr);

		mPs.clear();
	}
}

fPointPtrX Bezier::GetPoint(DWORD pos)
{	return Points[pos];		}

fPointPtrX Bezier::GetNextPoint()
{	return GetPoint(Pos++);	}

void Bezier::bind(xtal::ClassPtr it)
{
	USE_XDEFZ(Bezier);
	Xdef_method(GetPoint);
	Xdef_method(GetNextPoint);
}