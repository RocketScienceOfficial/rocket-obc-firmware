using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIButton : MonoBehaviour
{
    [SerializeField] private List<UIButtonData> m_Data;
    [SerializeField] private bool m_DeactivateAllPanels;

    private void Start()
    {
        var button = GetComponent<Button>();

        if (m_DeactivateAllPanels)
        {
            button.onClick.AddListener(() => PanelsController.Instance.DeactivateAllPanels());
        }

        foreach (var item in m_Data)
        {
            button.onClick.AddListener(() => PanelsController.Instance.ActivatePanel(item.panel, item.activate));
        }
    }
}

[Serializable]
public struct UIButtonData
{
    public Panel panel;
    public bool activate;
}