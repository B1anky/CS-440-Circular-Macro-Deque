#ifndef DEQUE
#define DEQUE
#include <algorithm>
#define Deque_DEFINE(t)									\
	bool (*t##_cmp)(const t & o1, const t & o2);					\
	struct Deque_##t##_Iterator;							\
	void inc(Deque_##t##_Iterator *self);						\
	void dec(Deque_##t##_Iterator *self);						\
	t& deref(Deque_##t##_Iterator *self);						\
	struct Deque_##t {								\
		size_t length;								\
		size_t max_length;							\
		t *array;								\
		struct Deque_##t##_Iterator *begin_ptr;					\
		struct Deque_##t##_Iterator *end_ptr;					\
		char type_name[6 + sizeof #t]= "Deque_" #t;				\
		size_t (*size)(Deque_##t *);						\
		void (*dtor)(Deque_##t *); 						\
		bool (*less_by_id)(const t &, const t &);				\
		bool (*empty)(Deque_##t *);						\
		void (*push_back)(Deque_##t *, t);					\
		void (*push_front)(Deque_##t *, t);					\
		void (*pop_front)(Deque_##t *);						\
		void (*pop_back)(Deque_##t *);						\
		void (*reallocate)(Deque_##t *);					\
		void (*clear)(Deque_##t *);						\
		t& (*front)(Deque_##t *);						\
		t& (*back)(Deque_##t *);						\
		t& (*at)(Deque_##t *, size_t);						\
		Deque_##t##_Iterator& (*begin)(Deque_##t *);				\
		Deque_##t##_Iterator& (*end)(Deque_##t *);				\
		bool (*Deque_##t##_equal)(Deque_##t o1, Deque_##t o2);			\
		void (*sort)(Deque_##t *, Deque_##t##_Iterator, Deque_##t##_Iterator);	\
	};										\
	void clear(Deque_##t *self);							\
	bool Deque_##t##_equal(Deque_##t o1, Deque_##t o2){				\
		if(o1.length != o2.length)						\
			return false;							\
		for(size_t i = 0; i < o1.length; i++){					\
			if(o1.less_by_id(o1.at(&o1, i), o2.at(&o2, i)) || o2.less_by_id(o2.at(&o2, i), o1.at(&o1, i)))	\
				return false;						\
		}									\
		return true;								\
	}										\
	struct Deque_##t##_Iterator{							\
		t* cur;									\
		t* container_begin;							\
		t* container_end;							\
		bool is_begin;								\
		void (*inc)(Deque_##t##_Iterator * self);				\
		void (*dec)(Deque_##t##_Iterator * self);				\
		t& (*deref)(Deque_##t##_Iterator *self);				\
		bool (*Deque_##t##_Iterator_equal)					\
			(Deque_##t##_Iterator self, Deque_##t##_Iterator other);	\
	};										\
	void dtor(Deque_##t *self) {							\
		free(self->array);							\
		free(self->begin_ptr);							\
		free(self->end_ptr);							\
	}       									\
	size_t size(Deque_##t *self){							\
		return self->length;							\
	}   										\
	bool empty(Deque_##t *self){							\
		return self->length == 0;						\
	}										\
	bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator self, Deque_##t##_Iterator other){\
		if(self.is_begin){							\
			if(self.cur == self.container_end - 1)				\
				return self.container_begin == other.cur;		\
			else								\
				return self.cur + 1 == other.cur;			\
		}else{									\
			if(self.cur == self.container_begin)				\
				return self.container_end - 1 == other.cur;		\
			else								\
				return self.cur - 1 == other.cur;			\
		}									\
	}										\
	void reallocate(Deque_##t *self) {						\
		size_t front_to_container_end = self->end_ptr->container_end - self->begin_ptr->cur;\
		self->max_length *= 2;							\
		t* doubled_array = (t*)malloc(sizeof(t) * self->max_length); 		\
		struct Deque_##t##_Iterator * doubled_begin = (Deque_##t##_Iterator *) malloc(sizeof(Deque_##t##_Iterator));\
		struct Deque_##t##_Iterator * doubled_end = (Deque_##t##_Iterator *) malloc(sizeof(Deque_##t##_Iterator));\
		doubled_begin->cur = doubled_array;					\
		doubled_end->cur = doubled_array + self->length;			\
		doubled_begin->container_begin = doubled_end->container_begin = doubled_array;	\
		doubled_begin->container_end = doubled_end->container_end = doubled_array + self->max_length;\
		std::copy(&(self->front(self)), self->begin_ptr->container_end, doubled_begin->cur);\
		std::copy(self->begin_ptr->container_begin, (&(self->back(self)) + 1), doubled_begin->cur + front_to_container_end - 1);\
		self->dtor(self);							\
		self->begin_ptr = doubled_begin;					\
		self->end_ptr = doubled_end;						\
		self->array = doubled_array;						\
		self->begin_ptr->cur = doubled_begin->container_end - 1;		\
		self->end_ptr->cur = doubled_end->container_begin + self->length;	\
		self->end_ptr->container_end = self->begin_ptr->container_end = self->array + self->max_length;	\
		self->end_ptr->container_begin = self->begin_ptr->container_begin = self->array;\
		self->begin_ptr->is_begin = true;					\
		self->end_ptr->is_begin = false;					\
		self->begin_ptr->inc = self->end_ptr->inc = &inc;			\
		self->begin_ptr->dec = self->end_ptr->dec = &dec;			\
		self->begin_ptr->deref = self->end_ptr->deref = &deref;			\
		self->begin_ptr->Deque_##t##_Iterator_equal = 				\
			self->end_ptr->Deque_##t##_Iterator_equal = &Deque_##t##_Iterator_equal;\
	} 										\
	void inc(Deque_##t##_Iterator *self){						\
		if(self->cur == self->container_end - 1)				\
			self->cur = self->container_begin;				\
		else									\
			(self->cur)++;							\
	}										\
	void dec(Deque_##t##_Iterator *self){						\
		if(self->cur == self->container_begin)					\
			self->cur = self->container_end - 1;				\
		else									\
			self->cur--;							\
	}										\
	t& deref(Deque_##t##_Iterator *self){						\
		if(self->is_begin){							\
			if(self->cur == self->container_end - 1)			\
				return *(self->container_begin);			\
			else								\
				return *(self->cur + 1);				\
		}else{									\
			if(self->cur == self->container_begin)				\
				return *(self->container_end - 1);			\
			else								\
				return *(self->cur); 					\
		}									\
	}										\
	t& front(Deque_##t * self){							\
		if(self->begin_ptr->cur == self->begin_ptr->container_end - 1)		\
			return *(self->begin_ptr->container_begin);			\
		else									\
			return *(self->begin_ptr->cur + 1);				\
	}										\
	t& back(Deque_##t * self){							\
		if(self->end_ptr->cur == self->end_ptr->container_begin)		\
			return *(self->end_ptr->container_end - 1);			\
		else									\
			return *(self->end_ptr->cur - 1);				\
	}										\
	Deque_##t##_Iterator& begin(Deque_##t * self){					\
		return *self->begin_ptr;						\
	}										\
	Deque_##t##_Iterator& end(Deque_##t * self){					\
		return *(self->end_ptr);						\
	}										\
	void push_back(Deque_##t *self, t item){					\
		if(self->end_ptr->cur == self->end_ptr->container_end - 1){		\
			*self->end_ptr->cur = item;					\
			self->end_ptr->cur = self->array;				\
			self->length++;							\
		}else{									\
			*self->end_ptr->cur = item;					\
			self->end_ptr->cur++;						\
			self->length++;							\
		}									\
		if(self->length == self->max_length)					\
			self->reallocate(self);						\
	}										\
	void push_front(Deque_##t *self, t item){					\
		if(self->begin_ptr->cur == self->begin_ptr->container_begin){		\
			*self->begin_ptr->cur = item;					\
			self->begin_ptr->cur = self->begin_ptr->container_end - 1;	\
			self->length++;							\
		}else{									\
			*self->begin_ptr->cur = item;					\
			self->begin_ptr->cur--;						\
			self->length++;							\
		}									\
		if(self->length == self->max_length)					\
			self->reallocate(self);						\
	}										\
	void pop_back(Deque_##t *self){							\
		dec(self->end_ptr);							\
		self->length--;								\
	}										\
	void pop_front(Deque_##t *self){						\
		inc(self->begin_ptr);							\
		self->length--;								\
	}										\
	t& at(Deque_##t *self, size_t i){						\
		if(&self->front(self) + i > self->end(self).container_end - 1){		\
			i -= self->end_ptr->container_end - &self->front(self) - 1;	\
			return *(self->array+ i);					\
		}									\
		return *(&self->front(self) + i) ;					\
	}										\
	int Deque_##t##_secret_cmp(const void* o1, const void* o2){			\
		if(t##_cmp(*((t*) o1), *((t*)o2)))					\
			return -1;							\
		else if(t##_cmp(*((t*) o2), *((t*)o1)))					\
			return 1;							\
		else									\
			return 0;							\
	}										\
	void sort(Deque_##t *self, Deque_##t##_Iterator begin, Deque_##t##_Iterator end){\
		t* buffer = (t*)malloc(sizeof(t) * self->max_length);			\
		size_t i = 0;								\
		if(self->begin(self).container_end != end.cur)				\
			end.inc(&end);							\
		for(auto it = begin; !Deque_##t##_Iterator_equal(it, end); it.inc(&it)){\
			buffer[i] = *it.cur;						\
			i++;								\
		}									\
		qsort(buffer, i, sizeof(t), &Deque_##t##_secret_cmp);			\
		i = 0;									\
		for(auto it = begin; !Deque_##t##_Iterator_equal(it, end); it.inc(&it), i++){\
			*it.cur = buffer[i];						\
		}									\
		free(buffer);								\
	}										\
	void Deque_##t##_ctor(Deque_##t *self, bool (*less)(const t &, const t &)) {	\
		self->length = 0;							\
		self->max_length = 4;							\
		self->begin_ptr = (Deque_##t##_Iterator *) malloc(sizeof(Deque_##t##_Iterator));\
		self->end_ptr =(Deque_##t##_Iterator *) malloc(sizeof(Deque_##t##_Iterator));\
		self->array = (t*)malloc(sizeof(t) * self->max_length);			\
		self->begin_ptr->cur = self->array;					\
		self->end_ptr->cur = self->array + 1;					\
		self->begin_ptr->container_begin = self->end_ptr->container_begin = self->array;\
		self->begin_ptr->container_end = self->end_ptr->container_end = self->array + self->max_length;\
		self->begin_ptr->inc = self->end_ptr->inc = &inc;			\
		self->begin_ptr->dec = self->end_ptr->dec = &dec;			\
		self->begin_ptr->deref = self->end_ptr->deref = &deref;			\
		self->begin_ptr->Deque_##t##_Iterator_equal = 				\
			self->end_ptr->Deque_##t##_Iterator_equal = &Deque_##t##_Iterator_equal;\
		self->begin_ptr->is_begin = true;					\
		self->end_ptr->is_begin = false;					\
		self->size = &size;							\
		self->less_by_id = less;						\
		self->dtor = &dtor;							\
		self->empty = &empty;							\
		self->push_back = &push_back;						\
		self->push_front = &push_front;						\
		self->pop_front = &pop_front;						\
		self->pop_back = &pop_back;						\
		self->reallocate = &reallocate;						\
		self->front = &front;							\
		self->back = &back;							\
		self->begin = &begin;							\
		self->end = &end;							\
		self->at = &at;								\
		self->clear = &clear;							\
		self->Deque_##t##_equal = &Deque_##t##_equal;				\
		self->sort = &sort;							\
		t##_cmp = less;								\
	}										\
	void clear(Deque_##t *self){							\
		self->dtor(self);							\
		Deque_##t##_ctor(self, self->less_by_id);				\
	}										
#endif
