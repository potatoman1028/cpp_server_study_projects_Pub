#ifndef  __Sync_Synchronized_H
#define  __Sync_Synchronized_H

namespace cpp
{
	namespace lang
	{
		class Object;

		class Synchronized
		{
		public:
					Synchronized( Object* obj );
			virtual	~Synchronized();

		private:
					Object*		monitorObject;
		};
	}
}

#endif
