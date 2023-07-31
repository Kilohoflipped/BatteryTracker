classdef DSP2PCConnector < handle

    properties
        timeOut
        dataProcessQueue
        clientAddress
        clientSocket
        connectionStatus
        serverIP
        serverPort
        serverSocket
        dataMutex
    end

    methods
        function obj = DSP2PCConnector(serverIP, serverPort, dataProcessQueue,dataMutex)
            % 构造函数，初始化服务器IP，端口，和一个处理数据的队列
            obj.timeOut = 10;
            obj.dataProcessQueue = dataProcessQueue;
            obj.clientAddress = [];
            obj.clientSocket = [];
            obj.connectionStatus = false;
            obj.serverIP = serverIP;
            obj.serverPort = serverPort;
            obj.serverSocket = tcpserver(serverIP, str2double(serverPort), "Timeout", 10);
            obj.serverSocket.ConnectionChangedFcn = @handleClient;
            configureTerminator(obj.serverSocket,238)
            obj.dataMutex = dataMutex;
        end
        function startServer(obj)
            % 启动服务器并开始监听
            fprintf('Server is listening on %s:%d', obj.serverIP, obj.serverPort);
            if ~obj.serverSocket.Connected
                fprintf('Server is listening on %s:%d', obj.serverIP, obj.serverPort);
                fopen(obj.serverSocket);
                fprintf('Server is listening on %s:%d', obj.serverIP, obj.serverPort);
                obj.checkConnection();
            end
        end
        %function checkConnection(obj)
            % 检查客户端连接状态
        %    if ~obj.connectionStatus
        %        fopen(obj.clientSocket); % 尝试打开连接
        %        fprintf('Connection from %s', obj.clientAddress);
        %        obj.connectionStatus = true;
        %        obj.handleClient();
        %    end
        %    pause(1); % 等待一秒再次检查
        %    obj.checkConnection(); % 递归调用自己，保持检查连接
        %end
        function handleClient(obj)
            % 处理客户端发送的数据
            if obj.serverSocket.Connected
                fprintf('Server is listening on %s:%d', obj.serverIP, obj.serverPort);
            end
            while obj.serverSocket.Connected
                buffer = java.util.LinkedList;
                while obj.clientSocket.BytesAvailable
                    data = fread(obj.clientSocket, obj.clientSocket.BytesAvailable);
                    buffer.addAll(data);
                end
                if buffer.size() >= 6
                    if buffer.get() == '!' && buffer.get(buffer.size()-1:buffer.size()) == '\r' && buffer.size() == 6
                        channel = buffer.get(1);
                        adcHigh = buffer.get(2);
                        adcLow2 = bitand(buffer.get(3), 3); % 只保留低2位
                        adcValue = bitshift(adcHigh, 2) | adcLow2; % 将高8位与低2位合并
                        adcResult = adcValue / 1023 * 5; % 计算ADC结果
                        % adcResult = adcResult * (200 + 510) / 510 / 25 * 15; % 电压分压
                        dataItem = struct('Source', channel, 'Time', datetime('now'), 'adcVoltage', adcResult);
                        if numel(obj.dataProcessQueue.dataQueue) == obj.dataProcessQueue.dataQueue.Capacity
                            obj.dataProcessQueue.dataQueue.removeElementAt();
                        end
                        obj.dataProcessQueue.dataQueue.add(dataItem); % 将数据加入队列
                        buffer.clear(); % 清空buffer
                    else
                        buffer.clear();
                    end
                end
                % 如果发生错误，标记连接状态为false并退出
                obj.connectionStatus = false;
                fclose(obj.clientSocket);
            end
        end
    end

end