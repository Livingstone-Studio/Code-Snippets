using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Interactable : MonoBehaviour
{
    public MeshFilter mF;

    public MeshRenderer mR;

    [SerializeField] protected string tooltipText = "";

    [SerializeField] protected Outline outline;

    // Start is called before the first frame update
    protected virtual void Start()
    {
        if (!mF)
        {
            mF = GetComponent<MeshFilter>();
        }

        if (!mR)
        {
            mR = GetComponent<MeshRenderer>();
        }

        if (!outline)
        {
            outline = GetComponent<Outline>();
        }

        SetHighlighted(false);
    }

    public void SetHighlighted(bool state)
    {
        if (outline) outline.enabled = state;

        if (InputHandler.Instance.inputType != InputType.FPS)
        {
            TooltipReader.Instance.ActivateTooltip(false, tooltipText);
        }
        else
        {
            TooltipReader.Instance.ActivateTooltip(state, tooltipText);
        }
    }

    public virtual void OnPickup()
    {
        SetHighlighted(false);
    }
}
