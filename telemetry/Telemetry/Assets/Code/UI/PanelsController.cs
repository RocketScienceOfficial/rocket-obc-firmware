using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PanelsController : MonoBehaviour
{
    public static PanelsController Instance { get; private set; }

    [Header("Panels")]
    [SerializeField] private GameObject m_DashboardPanel;
    [SerializeField] private GameObject m_PlotsPanel;
    [SerializeField] private GameObject m_MapPanel;
    [SerializeField] private GameObject m_3DPanel;
    [SerializeField] private GameObject m_CamPanel;
    [SerializeField] private GameObject m_PortSelectionPanel;

    [Header("Tabs")]
    [SerializeField] private UITab m_DashboardTab;
    [SerializeField] private UITab m_PlotsTab;
    [SerializeField] private UITab m_MapTab;
    [SerializeField] private UITab m_3DTab;
    [SerializeField] private UITab m_CamTab;

    private void Awake()
    {
        Instance = this;
    }

    private void Start()
    {
        DeactivateAllPanels();
        ActivatePanel(Panel.Dashboard, true);
    }

    public void DeactivateAllPanels()
    {
        m_DashboardPanel.SetActive(false);
        m_PlotsPanel.SetActive(false);
        m_MapPanel.SetActive(false);
        m_3DPanel.SetActive(false);
        m_CamPanel.SetActive(false);
        m_PortSelectionPanel.SetActive(false);

        m_DashboardTab.SetActivation(false);
        m_PlotsTab.SetActivation(false);
        //m_MapTab.SetActivation(false);
        m_3DTab.SetActivation(false);
        //m_CamTab.SetActivation(false);
    }

    public void ActivatePanel(Panel panel, bool activation)
    {
        GameObject panelObj = null;

        switch (panel)
        {
            case Panel.Dashboard:
                panelObj = m_DashboardPanel;

                m_DashboardTab.SetActivation(true);
                break;
            case Panel.Plots:
                panelObj = m_PlotsPanel;

                m_PlotsTab.SetActivation(true);
                break;
            case Panel.Map:
                panelObj = m_MapPanel;

                //m_MapTab.SetActivation(true);
                break;
            case Panel.Render:
                panelObj = m_3DPanel;

                m_3DTab.SetActivation(true);
                break;
            case Panel.Cam:
                panelObj = m_CamPanel;

                //m_CamTab.SetActivation(true);
                break;
            case Panel.PortSelection:
                panelObj = m_PortSelectionPanel;
                break;
            default:
                break;
        }

        panelObj.SetActive(activation);
    }
}

public enum Panel
{
    Dashboard,
    Plots,
    Map,
    Render,
    Cam,
    PortSelection
}