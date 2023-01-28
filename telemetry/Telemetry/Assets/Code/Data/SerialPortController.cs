using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.IO;
using UnityEngine;
using TMPro;
using UnityEngine.UI;
using System.Diagnostics;
using System.Threading;

using Debug = UnityEngine.Debug;

public class SerialPortController : MonoBehaviour
{
    private const int RP2040_PID = 0x000A;
    private const int RP2040_VID = 0x2E8A;

    public event EventHandler OnConnected;
    public event EventHandler OnDisconnected;
    public event EventHandler<OnReadDataEventArgs> OnReadData;

    [SerializeField] private TextMeshProUGUI m_PortText;
    [SerializeField] private TextMeshProUGUI m_StatusText;
    [SerializeField] private Button m_DisconnectButton;

    private readonly Queue<string> _serialReadQueue = new();
    private readonly object _serialReadQueueLock = new();
    private bool _closePort;
    private readonly object _closePortLock = new();
    private SerialPort _currentSerialPort;
    private Thread _serialReadThread;

    public bool Connected => _currentSerialPort != null;
    public string CurrentPath => _currentSerialPort?.PortName;

    private void Start()
    {
        UpdatePortUI();

        m_DisconnectButton.onClick.AddListener(() => Disconnect());
    }

    private void Update()
    {
        lock (_serialReadQueueLock)
        {
            if (_serialReadQueue.Count > 0)
            {
                OnReadData?.Invoke(this, new OnReadDataEventArgs { Data = _serialReadQueue.Dequeue() });
            }
        }

        lock (_closePortLock)
        {
            if (_closePort)
            {
                Disconnect();

                _closePort = false;
            }
        }
    }

    public void Connect(string port)
    {
        try
        {
            _currentSerialPort = new SerialPort()
            {
                PortName = port,
                BaudRate = 115200,
                Parity = Parity.None,
                DataBits = 8,
                StopBits = StopBits.One,
                RtsEnable = true,
                DtrEnable = true,
                ReadTimeout = 1000,
                WriteTimeout = 1000,
            };

            _currentSerialPort.Open();

            _serialReadThread = new Thread(SerialReadThread);
            _serialReadThread.Start();

            UpdatePortUI();

            OnConnected?.Invoke(this, EventArgs.Empty);

            Debug.Log("COM Port Connected!");
        }
        catch (Exception ex)
        {
            Debug.Log("Could not find serial port: " + ex);

            _currentSerialPort = null;
        }
    }

    private void Disconnect()
    {
        if (Connected)
        {
            _currentSerialPort.Close();
            _currentSerialPort = null;

            _serialReadThread.Join();
            _serialReadThread = null;

            OnDisconnected?.Invoke(this, EventArgs.Empty);

            UpdatePortUI();
        }
    }

    private void SerialReadThread()
    {
        while (Connected)
        {
            try
            {
                var message = _currentSerialPort.ReadLine();

                if (!string.IsNullOrEmpty(message))
                {
                    lock (_serialReadQueueLock)
                    {
                        _serialReadQueue.Enqueue(message);
                    }
                }
            }
            catch (TimeoutException) { }
            catch (InvalidOperationException) { }
            catch (IOException)
            {
                lock (_closePortLock)
                {
                    _closePort = true;
                }

                break;
            }
        }
    }

    private void UpdatePortUI()
    {
        if (Connected)
        {
            m_PortText.SetText(_currentSerialPort.PortName);
            m_StatusText.SetText("Connected");
        }
        else
        {
            m_PortText.SetText("None");
            m_StatusText.SetText("Disconnected");
        }

        m_DisconnectButton.interactable = Connected;
        m_DisconnectButton.GetComponentInChildren<TextMeshProUGUI>().SetText(Connected ? "Disconnect" : "Disconnected");
    }

    public IEnumerable<RP2040SerialPortData> ListRP2040SerialPorts()
    {
        var startInfo = new ProcessStartInfo
        {
            WindowStyle = ProcessWindowStyle.Hidden,
            CreateNoWindow = true,
            UseShellExecute = false,
            FileName = "cmd.exe",
            Arguments = "/C cd Utils && python listComPorts.py",
            RedirectStandardOutput = true,
        };

        var process = Process.Start(startInfo);

        process.WaitForExit();

        var output = process.StandardOutput.ReadToEnd();
        var lines = output.Split(',', StringSplitOptions.RemoveEmptyEntries);

        foreach (var line in lines)
        {
            var data = line.Split('|');
            var port = data[0];
            var pid = int.Parse(data[1]);
            var vid = int.Parse(data[2]);

            if (pid == RP2040_PID && vid == RP2040_VID)
            {
                yield return new RP2040SerialPortData { name = port, pid = pid, vid = vid };
            }
        }
    }
}

public class RP2040SerialPortData
{
    public string name;
    public int vid;
    public int pid;
}

public class OnReadDataEventArgs : EventArgs
{
    public string Data { get; set; }
}