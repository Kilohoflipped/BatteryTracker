classdef wlanIPV4Address
    %UNTITLED2 此处提供此类的摘要
    %   此处提供详细说明

    properties
        adapterName     % 适配器名称
        ipv4Address     % IPv4 地址
    end

    methods
        function obj = untitled2(inputArg1,inputArg2)
            %UNTITLED2 构造此类的实例
            %   此处提供详细说明
            obj.Property1 = inputArg1 + inputArg2;
        end

        function outputArg = method1(obj,inputArg)
            %METHOD1 此处提供此方法的摘要
            %   此处提供详细说明
            outputArg = obj.Property1 + inputArg;
        end
    end
end