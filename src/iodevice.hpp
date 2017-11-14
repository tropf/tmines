#ifndef __IODEVICE_H_INCLUDED__
#define __IODEVICE_H_INCLUDED__

class IODevice {
    public:
        virtual ~IODevice() {} 

        /**
         * Returns a pressed key to be handled by the display class.  
         * Is blocking.
         * @returns the pressed key
         */
        virtual char getChar() = 0;
};

#endif // __IODEVICE_H_INCLUDED__
