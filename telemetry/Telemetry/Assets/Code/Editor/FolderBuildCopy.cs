using System.Collections;
using System.Collections.Generic;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEngine;

public class FolderBuildCopy : IPostprocessBuildWithReport
{
    public int callbackOrder => 1;

    public void OnPostprocessBuild(BuildReport report)
    {
        IOUtils.EnsureDirectoryExists("Builds/Windows/Utils/");
        IOUtils.CopyFilesRecursively("Utils/", "Builds/Windows/Utils/");
    }
}