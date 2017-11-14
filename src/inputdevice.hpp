#ifndef __INPUTDEVICE_H_INCLUDED__
#define __INPUTDEVICE_H_INCLUDED__

class Inputdevice {
    public:
        virtual ~Inputdevice() {} 

        /**
         * Returns a pressed key to be handled by the display class.  
         * Is blocking.
         * @returns the pressed key
         */
        virtual char getChar() = 0;
};

#endif // __INPUTDEVICE_H_INCLUDED__
