classdef wlanIPV4Address
    %wlanIPV4Address 此处提供此类的摘要
    %   获取当前计算机的无线局域网适配器的IPv4地址

    properties
        adapterName     % 适配器名称
        wlanStatus      % 适配器状态
        ipv4Address     % IPv4 地址
    end

    methods
        function obj = wlanIPV4Address()
            %wlanIPV4Address 构造此类的实例
            obj.adapterName = '';
            obj.ipv4Address = '';
            obj.wlanStatus = '';
            obj = obj.getIPV4Address();
        end

        function obj = getIPV4Address(obj)
            % 运行ipconfig指令
            [~, result] = system('ipconfig');
            result = splitlines(result);
            result = result(~cellfun('isempty', result));

            % 在结果中找到无线局域网适配器的IPv4地址
            for i = 1:numel(result)
                line = result{i};
                if contains(line, 'Wireless LAN') || contains(line, 'Wi-Fi') || contains(line, 'WLAN')
                    % 获取适配器名称
                    obj.adapterName = strrep(line,':','');
                    for j = i+1:numel(result)
                        % 获取IPv4地址
                        nextLine = result{j};
                        if contains(nextLine, 'IPv4')
                            tokens = strsplit(nextLine, ': ');
                            obj.ipv4Address = tokens{2};  % 找到局域网IPv4地址
                            obj.wlanStatus = "200 OK";  % 成功找到IPv4地址
                            return;
                        elseif contains(nextLine, '媒体已断开连接')
                            obj.ipv4Address = 'WLAN 已断开连接';  % 发现WLAN适配器已断开连接
                            obj.wlanStatus = "502 Bad Gateway";  % 发现WLAN适配器断开
                            return;
                        end
                    end
                end
            end
            % 遍历了每一行，没有发现WLAN适配器
            obj.ipv4Address = '未找到 WLAN 适配器';
            obj.wlanStatus = "404 Not Found";  % 未找到WLAN适配器
        end
        
    end
end