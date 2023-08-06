classdef serialDataProcess

    properties
        startIndex
        serialRlDta
        warnChannelNum=0
    end

    methods
        function obj = serialDataProcess()
            %obj.startIndex = findStartIndex(serialData);
            %obj.serialRlDta = getRlDta(serialData);
        end

        function startIndex = findStartIndex(~,serialData)
            for i = 1:length(serialData)
                if serialData(i) == double(255)
                    % 在找到'A'后，检查后续两个元素是否与目标数字组合匹配
                    if i+1 <= length(serialData) && serialData(i+1) == double(240)
                    %if i+2 <= length(serialData) && serialData(i+1) == double('A') && serialData(i+2) == double('D')
                        startIndex = i;
                        return;
                    end
                end
            end
        end

        function serialRlDta = getRlDta(obj,serialData,ratedChannelNum)
            for i = 1:ratedChannelNum
                serialRlDta{i} = [];
            end
            for i = obj.startIndex:length(serialData)
                if serialData(i) == double(255)
                    % 寻找起始位
                    if i+1 <= length(serialData) && serialData(i+1) == double(240)
                    %if i+2 <= length(serialData) && serialData(i+1) == double('A') && serialData(i+2) == double('D')
                        % 判断是否
                        if i+ratedChannelNum*2+3 > length(serialData) % 已经末尾了
                            return;
                        end
                        %channelNum = serialData(i+3)-208;   %减去0XD0的偏置，获得当前次数使用的通道数

                        %if channelNum ~= ratedChannelNum
                        %    obj.warnChannelNum = 1;  % 警告参考通道数与实际通道数不符
                        %else
                        %    obj.warnChannelNum = 0;
                        %end

                        dataLengthFlag = ratedChannelNum*2;      %算一下索引偏置

                        if serialData(i+dataLengthFlag+2) ~= 255 || serialData(i+dataLengthFlag+3) ~= 240   % 下一组的起始位有任何一个不符合
                            continue;
                        end
                        for j=1:ratedChannelNum
                            low8bits = uint16( serialData(i+2+(j-1)*2) ); %低八位
                            high8bits = uint16(serialData(i+3+(j-1)*2) ); %高四位
                            rlnumber = obj.bits2number(low8bits,high8bits,12,3);
                            serialRlDta{j} = [serialRlDta{j} rlnumber];
                        end
                    end
                end
            end
        end

        function rlNumber = bits2number(~,low8bits,high8bits,resolution,vRefHi)
            % resolution 采样深度
            switch resolution
                case 12 % 采样深度为12位
                    high8bits = bitand(high8bits,uint16(15));       % 保留右边4位
                    high8bits = bitshift(high8bits, 8);             % 左移八位
                    rlNumber = high8bits+low8bits;                  % 得到最终的数字
                    rlNumber = double(rlNumber)/4096*vRefHi;        % 转换得到浮点数
            end
        end
    end
end