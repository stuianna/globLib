#ifndef _FIFO_HPP
#define _FIFO_HPP

#include <cstdint>

/**
  @addtogroup Common
  @{
*/

/*!
    @addtogroup FIFO
    @brief Circular FIFO buffer template class
		@details There are two modes of operation for implementation.
			1. **Automatic output** - The first entry of the fifo is sent to the output
			function as soon as the first entry is written with put. Successive
			entries can be retreived by using the get function.
			2. **Triggered output** - Contents are continually added the the buffer.
			The size of the buffer can be monitored and all contents sent to the output
			function using the flush function.

			An item is not added to the buffer if it is already full.

    @author Stuart Ianna
    @date Novamber 2019
    @warning FIFO memory buffer size must be a exponention power of 2 (2,4,8,16 ..)

    @par Example of using FIFO to overcome blocking TX (at low baud rates) and usart.
		In the example, the byte are taken from the FIFO buffer automattically using the
		UART TX ISR.
    @include example_fifo_usartTX.cpp

		@par Example of using FIFO for USART RX.
		The contents of the fifo are retreived after 4 bytes are available.
    @include example_fifo_usartRX.cpp

		@par Example of using FIFO for USART TX in combination with printf.
		The FIFO template is also used for a structre type, making a nested
		FIFO setup.
    @include example_fifo_multiple.cpp
  @{
*/

template <typename T>
class cFIFO{

public:

/*!
    @brief Fifo modes of operation.
*/
	typedef enum{
		FIFO_MODE_AUTO,							//!< Fifo contents are dumped automatically.
		FIFO_MODE_TRIGGER,					//!< Fifo contents are only dumped with #get or #flush
	}Fifo_Mode;

/*!
    @brief Fifo return types
*/
	typedef enum{
		FIFO_RETURN_ERROR,					//!< An error occured
		FIFO_RETURN_OK,							//!< No error occured
	}Fifo_Return;

/*!
    @brief Contructor, set up a new FIFO object.
    @param buffer Pointer to buffer memory location. Must be an exponential
		power of 2. (2,4,8,16,32 ..)
    @param mode The mode of operation.
		@param size Must be the same size as the allocated memory location.
		@param output This function is called when #get of #flush is used.
    @return N/A
*/
	cFIFO(T *buffer, Fifo_Mode mode, uint16_t size, void (*output)(T));
/*!
    @brief Add an item to the memory buffer
		@param in The item to add. Note the item will not be added if the buffer
		is already full.
    @return The size of the buffer after the item was added.
*/
	void put(T in);
/*!
    @brief Send the next item to the output function.
    @return None
*/
	void get();
/*!
    @brief Get the current size used in the buffer
    @return None
*/
	uint16_t size();
/*!
    @brief Send all the buffer contents to the output function.
		@details Uses a while loop to push until all objects are sent.
*/
	void flush();
/*!
    @brief Reset the buffer.
		@details This effectivly sets the head and tail locations to the
		same address. It doesn't clear each memory location (set to zero.)
*/
	void reset();

private:
	T *buffer;									//!< A pointer to the start of the buffer memory location.
	uint16_t head;							//!< Relative location of the start of the buffer.
	uint16_t tail;							//!< Relative location of the end of the buffer.
	uint16_t mask;							//!< Mask to 'wrap around' the buffer
	uint16_t mode;							//!< The current mode of the object.
	bool idle;									//!< FIFO object status
	void (*output)(T);					//!< The output function
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

	this->head = 1;
	this->tail = 1;
	this->idle = true;
}

template<typename T>
void cFIFO<T>::put(T in){

	if((this->head) == ((this->tail-1) & this->mask)){

		return;
	}

	this->buffer[this->head++] = in;
	this->head = (this->head & this->mask);

	if((this->idle) && (this->mode == FIFO_MODE_AUTO)){
		this->get();
	}
}

template<typename T>
void cFIFO<T>::get(){

	if(this->head == this->tail){
		this->idle = true;
		return;
	}
	this->idle = false;

	this->output(this->buffer[this->tail++]);
	this->tail = (this->tail & this->mask);
}

template<typename T>
uint16_t cFIFO<T>::size(){

	return (this->head - this->tail) & this->mask;
}

template<typename T>
void cFIFO<T>::flush(){

	bool empty = false;
	while(!empty){

		this->get();
		empty = this->idle;
	}
}

/**@}*/
/**@}*/

#endif
