using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEngine;

public class RLFileSaver : DataRecipient
{
    private const string FILES_DIR = "Flight/";

    [SerializeField] private SerialPortController m_SerialPortController;

    private readonly StringBuilder _csvBuilder = new();

    private StreamWriter _writer;

    private void Start()
    {
        m_SerialPortController.OnConnected += (sender, args) => CreateFile();
        m_SerialPortController.OnDisconnected += (sender, args) => CloseFile();
    }

    private void CreateFile()
    {
        var datetime = DateTime.Now;
        var path = FILES_DIR + $"RealTimeFlightLog_{datetime.Hour}{datetime.Minute}{datetime.Second}.csv";

        IOUtils.EnsureDirectoryExists(path);

        _writer = new StreamWriter(path);
    }

    private void CloseFile()
    {
        if (_writer != null)
        {
            _writer.Close();
            _writer = null;

            //var startInfo = new ProcessStartInfo
            //{
            //    WindowStyle = ProcessWindowStyle.Hidden,
            //    CreateNoWindow = true,
            //    UseShellExecute = false,
            //    FileName = "cmd.exe",
            //    Arguments = "/C cd Utils && python reportGen.py",
            //};

            //var process = Process.Start(startInfo);

            //process.WaitForExit();
        }
    }

    private void WriteFileValue(float value)
    {
        _csvBuilder.Append(value);
        _csvBuilder.Append(",");
    }

    private void WriteFileValue(int value)
    {
        _csvBuilder.Append(value);
        _csvBuilder.Append(",");
    }

    public override void OnSetData(RecipentData data)
    {
        if (_writer != null)
        {
            WriteFileValue(data.accelerationX);
            WriteFileValue(data.accelerationY);
            WriteFileValue(data.accelerationZ);
            WriteFileValue(data.velocityX);
            WriteFileValue(data.velocityY);
            WriteFileValue(data.velocityZ);
            WriteFileValue(data.positionX);
            WriteFileValue(data.positionY);
            WriteFileValue(data.positionZ);
            WriteFileValue(data.roll);
            WriteFileValue(data.pitch);
            WriteFileValue(data.yaw);
            WriteFileValue(data.latitude);
            WriteFileValue(data.longitude);
            WriteFileValue(data.altitude);
            WriteFileValue(data.temperature);
            WriteFileValue(data.pressure);
            WriteFileValue(data.humidity);
            WriteFileValue(data.radioSignalStrength);
            WriteFileValue(data.cpuTemp);
            WriteFileValue(data.batteryPercentage);
            WriteFileValue(data.fuelPercentage);

            _writer.WriteLine(_csvBuilder.ToString());
            _csvBuilder.Clear();
        }
    }
}