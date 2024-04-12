using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class TooltipReader : MonoBehaviour
{

    public static TooltipReader Instance { set; get; }

    [SerializeField] private TextMeshProUGUI tooltip;
    [SerializeField] private GameObject tooltipGO;

    private void Awake()
    {
        if (!Instance)
        {
            Instance = this;
        }
    }

    public void ActivateTooltip(bool state, string message)
    {
        if (tooltipGO.activeInHierarchy != state) tooltipGO.SetActive(state);
        tooltip.text = message;
    }
}
