/* File: Sequence.cpp
 * Author: CRE
 * Last Edited: Wed Jun 15 17:36:18 2016
 */

#include "Sequence.h"
#include <cassert>
using namespace CREBIL;

Sequence::~Sequence()
{
	if (ReadOnly)
	{
		SHub.remove(SeqEntry);
	}
	else
	{
		delete Seq;
	}
}

Sequence::Sequence(const Sequence & that)
{
	Length=that.Length;
	ReadOnly=that.ReadOnly;
	Start=that.Start;
	if (ReadOnly)
	{
		SeqEntry=that.SeqEntry;
		SHub.copy(SeqEntry);
	}
	else
	{
		Size=Length+1;
		Seq=new char[Size];
		memcpy(Seq,that.Seq,Size);
	}
}

Sequence& Sequence::operator=(const Sequence & that)
{
	Length=that.Length;
	Start=that.Start;
	if(ReadOnly)
	{
		ReadOnly=that.ReadOnly;
		if (that.ReadOnly)
		{
			if(SeqEntry!=that.SeqEntry)
			{
				SHub.remove(SeqEntry);
				SeqEntry=that.SeqEntry;
			}
		}
		else
		{
			SHub.remove(SeqEntry);
			Size=Length+1;
			Seq=new char [Size];
			memcpy(Seq,that.Seq,Size);
		}
	}
	else
	{
		ReadOnly=that.ReadOnly;
		if (that.ReadOnly)
		{
			delete Seq;
			SeqEntry=that.SeqEntry;
			SHub.copy(SeqEntry);
		}
		else
		{
			if (Size>Length)
			{
				memcpy(Seq,that.Seq,Length+1);
			}
			else
			{
				delete Seq;
				Size=Length+1;
				Seq=new char [Size];
				memcpy(Seq,that.Seq,Size);
			}
		}
	}
}

Sequence Sequence::subSequence(unsigned begin, unsigned length)
{
	assert(begin<Length);
	if (begin+length>Length) length=Length-begin;
	Sequence sub=Sequence();
	sub.Length=length;
	sub.ReadOnly=ReadOnly;
	if (ReadOnly)
	{
		sub.SeqEntry=SeqEntry;
		SHub.copy(SeqEntry);
		sub.Start=Start+begin;
	}
	else
	{
		sub.Size=sub.Length+1;
		sub.Seq=new char[Size];
		memcpy(sub.Seq,Seq+begin,length);
		sub.Seq[length]='\0';
	}
	return sub;
}

const char & Sequence::operator[] (unsigned index)
{
	if (ReadOnly)
	{
		return SeqEntry->Seq[Start+index];
	}
	else
	{
		return Seq[index];
	}
}

Sequence & Sequence::makeEditable()
{
	if (ReadOnly)
	{
		if (SeqEntry->Occurs==1)
			Seq=(char *) SeqEntry->Seq;
		SeqEntry->Seq=nullptr;
		SHub.remove(SeqEntry);
		ReadOnly=false;
	}
	return *this;
}

Sequence & Sequence::makeReadOnly()
{
	if (ReadOnly) return *this;
	SeqEntry=SHub.newEntry();
	SeqEntry->Seq=Seq;
	SeqEntry->Occurs=1;
	Seq=nullptr;
	Start=0;
	ReadOnly=true;
	return *this;
}

