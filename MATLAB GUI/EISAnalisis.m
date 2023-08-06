classdef EISAnalisis
    %UNTITLED 此处提供此类的摘要
    %   此处提供详细说明

    properties
        serialHelper;
        warnTbprdOver
    end

    methods
        function obj = EISAnalisis(fInMin,finMax)
            %UNTITLED 构造此类的实例
            %   此处提供详细说明
            fIn = fInMin:finMax;
            lengthF = length(fIn);
            TBPRD = zeros(lengthF);

            for i=fInMin:finMax:1
                TBPRD(i) = 200*1000*1000/2/i;  %系统频率经过二分频后除以目标频率就是寄存器应有的值
                if TBPRD(i) > 65535
                    obj.warnTbprdOver = 1;
                end
            end
            serialHelper = obj.serialConnectCreat();
            for i=fInMin:finMax:1
                write(serialHelper,uint8('E'),"uint8");
                write(serialHelper,uint8('I'),"uint8");
                write(serialHelper,uint8('S'),"uint8");

                tbprdLow8 = uint16(TBPRD(i));
                tbprdLow8 = bitand(tbprdLow8,0xff);
                tbprdHigh8 = uint16(TBPRD(i));
                tbprdHigh8 = bitshift(tbprdHigh8,-8);
                write(tbprdLow8);
                write(tbprdHigh8);
                read(1000*);
            end

        end

        function serialHelper = serialConnectCreat(~)
            %METHOD1 此处提供此方法的摘要
            %   此处提供详细说明
            serialHelper = serialport("COM11",115200, ...
                                      "Parity","none", ...
                                      "DataBits",8, ...
                                      "StopBits",1);
        end

    end
end