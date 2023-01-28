using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class PortsController : MonoBehaviour
{
    [SerializeField] private GameObject m_MicrocontrollerPanel;
    [SerializeField] private Transform m_Parent;
    [SerializeField] private Button m_RefreshButton;
    [SerializeField] private SerialPortController m_SerialPortController;

    private void Start()
    {
        m_RefreshButton.onClick.AddListener(() => FetchPorts());
        m_SerialPortController.OnConnected += (sender, args) => FetchPorts();
        m_SerialPortController.OnDisconnected += (sender, args) => FetchPorts();

        FetchPorts();
    }

    private void FetchPorts()
    {
        Clear();

        foreach (var port in m_SerialPortController.ListRP2040SerialPorts())
        {
            SetupMicrocontroller(port.name);
        }
    }

    private void Clear()
    {
        for (int i = 0; i < m_Parent.childCount; i++)
        {
            Destroy(m_Parent.GetChild(i).gameObject);
        }
    }

    private void SetupMicrocontroller(string path)
    {
        var obj = Instantiate(m_MicrocontrollerPanel, m_Parent);

        obj.transform.Find("Image").GetComponentInChildren<TextMeshProUGUI>().SetText(path);
        obj.transform.Find("Connect Button").GetComponentInChildren<TextMeshProUGUI>().SetText(m_SerialPortController.CurrentPath == path ? "CONNECTED" : "CONNECT");
        obj.transform.Find("Connect Button").GetComponent<Button>().interactable = m_SerialPortController.CurrentPath != path;
        obj.transform.Find("Connect Button").GetComponent<Button>().onClick.AddListener(() => m_SerialPortController.Connect(path));
    }
}