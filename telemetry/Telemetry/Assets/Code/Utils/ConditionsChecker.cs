using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;

public class ConditionsChecker : MonoBehaviour
{
    private const string VALIDATED_KEY = "Validated";

    private void Start()
    {
        if (PlayerPrefs.GetInt(VALIDATED_KEY) == 0)
        {
            if (!CheckConditions())
            {
                UnityEngine.Debug.LogError("Couldn't validate app!");
            }

            PlayerPrefs.SetInt(VALIDATED_KEY, 1);
        }
    }

    private bool CheckConditions()
    {
        return CheckPython() && InstallDependencies();
    }

    private bool CheckPython()
    {
        var start = new ProcessStartInfo
        {
            WindowStyle = ProcessWindowStyle.Hidden,
            FileName = "cmd.exe",
            Arguments = "/C python --version",
            UseShellExecute = false,
            RedirectStandardError = true,
            RedirectStandardOutput = true,
            CreateNoWindow = true,
        };

        var process = Process.Start(start);

        process.WaitForExit();

        var output = process.StandardOutput.ReadToEnd();
        var result = !string.IsNullOrEmpty(output);

        return result;
    }

    private bool InstallDependencies()
    {
        var start = new ProcessStartInfo
        {
            WindowStyle = ProcessWindowStyle.Hidden,
            FileName = "cmd.exe",
            Arguments = "/C cd Utils && pip install -r requirements.txt",
            CreateNoWindow = true,
        };

        var process = Process.Start(start);

        process.WaitForExit();

        return true;
    }
}