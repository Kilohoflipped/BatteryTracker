classdef serialDataProcess

    properties
        startIndex
        serialRlDta
    end

    methods
        function obj = serialDataProcess()
            %obj.startIndex = findStartIndex(serialData);
            %obj.serialRlDta = getRlDta(serialData);
        end

        function startIndex = findStartIndex(~,serialData)
            for i = 1:length(serialData)
                if serialData(i) == double('A')
                    % 在找到'A'后，检查后续两个元素是否与目标数字组合匹配
                    if i+2 <= length(serialData) && serialData(i+1) == double('D') && serialData(i+2) == double('=')
                        startIndex = i;
                        return;
                    end
                end
            end
        end

        function serialRlDta = getRlDta(obj,serialData)
            serialRlDta = [];
            for i = obj.startIndex:length(serialData)
                if serialData(i) == double('A')
                    % 寻找起始位
                    if i+2 <= length(serialData) && serialData(i+1) == double('D') && serialData(i+2) == double('=')
                        % 判断是否遇到错误帧或者漏帧
                        if i+7 > length(serialData) % 已经末尾了
                            return;
                        end
                        if serialData(i+5) ~= 'A' || serialData(i+6) ~= 'D' || serialData(i+7) ~= '='   % 下一组的起始位有任何一个不符合
                            continue;
                        end
                        temp = uint16(serialData(i+4));     %高四位
                        temp = bitshift(temp, 8);               %低八位
                        temp = temp + uint16(serialData(i+3));
                        temp = double(temp)/4096*3.3;
                        serialRlDta = [serialRlDta temp];
                    end
                end
            end
        end
    end
end