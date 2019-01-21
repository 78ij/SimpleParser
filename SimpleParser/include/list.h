#ifndef SIMP_LIST_H
#define SIMP_LIST_H

#include <malloc.h>
namespace simple {
	template <class T>
	class list {
	public:
		T **data;
		list() {
			data = (T **)malloc(100 * sizeof(T*));
			size = 100;
			length = 0;
		}
		~list() {
			for (int i = 0; i < length; i++) {
				delete (data[i]);
			}
			free(data);
		}
		size_t size;
		size_t length;
		T & operator[](int index) {
			return *(data[index]);
		}
		void append(T &ele) {
			if (length == size) {
				data = (T **)realloc(data, (size + 100) * sizeof(T*));
			}
			T *tmp = new T;
			*tmp = ele;
			data[length++] = tmp;
		}
		T remove() {
			T tmp = *(data[length - 1]);
			length--;
			return tmp;
		}
		list(list<T> &list) = delete;
	};
}

#endif