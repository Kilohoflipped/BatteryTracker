classdef SerialReader < handle
    properties
        adresult = '';
    end
    
    methods
        function obj = SerialReader
            s = serial('COM1');  % 请将COM1替换为实际的串口号
            set(s,'BaudRate',9600);
            set(s,'DataBits',8);
            set(s,'StopBits',1);
            set(s,'Parity','none');
            set(s,'FlowControl','none');
            
            fopen(s);
            bytesRead = 0;
            bufferSize = 3000;  % 定义缓冲区大小为3000字节
            while bytesRead < bufferSize
                bytesAvailable = s.BytesAvailable;
                if bytesAvailable > 0
                    data = fread(s, bytesAvailable, 'uint8');
                    obj.adresult = [obj.adresult char(data')];
                    bytesRead = bytesRead + bytesAvailable;
                end
            end
            
            fclose(s);
            delete(s);
            clear s;
        end
    end
end