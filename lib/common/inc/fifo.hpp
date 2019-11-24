#ifndef _FIFO_HPP
#define _FIFO_HPP

#include <cstdint>

template <typename T>
class cFIFO{

public:

	typedef enum{
		FIFO_MODE_AUTO,
		FIFO_MODE_TRIGGER,
		FIFO_MODE_DUMP,
	}Fifo_Mode;

	typedef enum{
		FIFO_RETURN_ERROR,
		FIFO_RETURN_OK,
	}Fifo_Return;

	cFIFO(T *buffer, Fifo_Mode mode, uint16_t size, void (*output)(T));
	uint16_t put(T);
	uint16_t get();
	uint16_t size();
	void flush();
	void reset();

private:
	T *buffer;
	uint16_t head;
	uint16_t tail;
	uint16_t mask;
	uint16_t mode;
	bool idle;
	void (*output)(T);
};


template<typename T>
cFIFO<T>::cFIFO(T *buffer, Fifo_Mode mode, uint16_t size, void (*output)(T)){

	this->buffer = buffer;
	this->head = 0;
	this->tail = 0;
	this->mask = size-1;
	this->mode = mode;
	this->output = output;
	this->idle = true;
}

template<typename T>
void cFIFO<T>::reset (void){

	this->head = 0;
	this->tail = 0;
	this->idle = true;
}

template<typename T>
uint16_t cFIFO<T>::put(T in){

	if((this->head) == ((this->tail-1) & this->mask)){

		return this->mask;
	}

	this->buffer[this->head++] = in;
	this->head = (this->head & this->mask);

	if((this->idle) && (this->mode == FIFO_MODE_AUTO)){
		this->get();
	}
	return this->size();
}

template<typename T>
uint16_t cFIFO<T>::get(){

	if(this->head == this->tail){
		this->idle = true;
		return 0;
	}
	this->idle = false;

	this->output(this->buffer[this->tail++]);
	this->tail = (this->tail & this->mask);
	return this->size();
}

template<typename T>
uint16_t cFIFO<T>::size(){

	return (this->head-this->tail)<0?(this->tail-this->head):(this->head-this->tail);
}

template<typename T>
void cFIFO<T>::flush(){

	bool empty = false;
	while(!empty){

		this->get();
		empty = this->idle;
	}
}

#endif
