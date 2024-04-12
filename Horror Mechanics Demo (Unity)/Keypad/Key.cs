using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class Key : Interactable
{
    [SerializeField] private UnityEvent onPress;

    internal Interactable interactable;

    private bool pressing = false;

    public Vector3 pressedOffset;
    private Vector3 basePos;

    protected override void Start()
    {
        base.Start();

        basePos = transform.localPosition;

        interactable = GetComponent<Interactable>();

    }

    private void Update()
    {
        if (pressing)
        {
            transform.localPosition = basePos + pressedOffset;
        }
        else
        {
            transform.localPosition = basePos;
        }
    }

    public void Pressed()
    {
        onPress.Invoke();
        OnPress();
    }

    public void Activate()
    {
        interactable.enabled = true;
    }

    public void Deactivate()
    {
        transform.localPosition = basePos;
        interactable.SetHighlighted(false);
        interactable.enabled = false;
    }

    private void OnPress()
    {
        StartCoroutine(Press());
    }

    private IEnumerator Press()
    {
        if (pressing)
        {
            // Reset key pos
            transform.localPosition = basePos;
        }

        pressing = true;

        yield return new WaitForSeconds(0.2f);

        pressing = false;
    }
}