classdef serialReader < handle
    
    properties
        serialHelper;
        serialData
    end
    
    methods
        function obj = serialReader(bufferSize)
            obj.serialData = obj.recieveData(bufferSize);
        end

        function serialConnectCreat(obj)
            obj.serialHelper = serialport("COM11",115200, ...
                                          "Parity","none", ...
                                          "DataBits",8, ...
                                          "StopBits",1);
        end

        function serialData = recieveData(obj,bufferSize)
            obj.serialConnectCreat();
            serialData = read(obj.serialHelper,bufferSize,"uint8");
            obj.endSerialConnect();
        end

        function endSerialConnect(obj)
            flush(obj.serialHelper)
            delete(obj.serialHelper);
        end
    end
end