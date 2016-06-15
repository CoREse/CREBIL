#pragma once
#include <list>
#include <cstring>
namespace CREBIL {
	class Sequence
	{
		class SequenceHub;
		class HubEntry {
			const char * Seq;
			unsigned long long Occurs;
			HubEntry()
				:Seq(nullptr), Occurs(0)
			{}
			HubEntry(const HubEntry &)=delete;
			HubEntry & operator=(const HubEntry &)=delete;
			~HubEntry ()
			{
				delete Seq;
			}
			void setSeq(const char * data)
			{
				unsigned datalen=strlen(data)+1;
				Seq=new char [datalen];
				memcpy((char *)Seq,data,datalen);
			}
			friend class SequenceHub;
			friend class Sequence;
		};
		//to store const sequnece strings
		static class SequenceHub {
			std::list <HubEntry *> hub;
			SequenceHub ()
				:hub()
			{
			}
			HubEntry* newEntry (const char * data=nullptr)
			{
				hub.push_back(new HubEntry());
				if (data==nullptr) return hub.back();
				hub.back()->setSeq(data);
				hub.back()->Occurs=1;
				return hub.back();
			}
			HubEntry * copy(HubEntry * origin)
			{
				++origin->Occurs;
				return origin;
			}
			HubEntry * remove(HubEntry * origin)
			{
				--origin->Occurs;
				if (origin->Occurs==0)
				{
					delete origin;
					hub.remove(origin);
				}
				return origin;
			}
			friend class Sequence;
		} SHub;
		static const bool DefaultReadOnly=true;
		bool ReadOnly;
		unsigned Length;
		//for editable
		char * Seq;
		unsigned Size;
		//for readonly
		unsigned Start;
		HubEntry * SeqEntry;
		public:
		Sequence()
			:ReadOnly(DefaultReadOnly), Seq(nullptr), SeqEntry(nullptr), Length(0), Size(0), Start(0)
		{
		}
		Sequence (char * data, bool ro=DefaultReadOnly)
			:ReadOnly(ro)
		{
			Length=strlen(data);
			Start=0;
			if(ReadOnly)
			{
				SeqEntry=SHub.newEntry(data);
				Size=0;
			}
			else
			{
				Size=Length+1;
				Seq=new char[Size];
				memcpy(Seq,data,Size);
			}
		}
		~Sequence();
		Sequence (const Sequence &);
		Sequence & operator=(const Sequence &);
		Sequence(Sequence &&)= default;
		Sequence subSequence(unsigned begin, unsigned length);//to gain [begin, begin+length>length?length:begin+length) sub sequence;
		const char & operator[] (unsigned index);
		Sequence & makeEditable();
		Sequence & makeReadOnly();
	};
}
