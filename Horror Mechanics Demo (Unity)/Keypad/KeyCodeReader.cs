using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using TMPro;

public enum PadState{ INTERACTABLE, ACTIVE }

public class KeyCodeReader : Interactable
{

    public PadState padState;

    [SerializeField] private UnityEvent onCorrect;

    [SerializeField] private TextMeshProUGUI screen;

    [SerializeField] private int code = 1234;

    [SerializeField] private Color baseColour;
    [SerializeField] private Color correctColour;
    [SerializeField] private Color incorrectColour;

    public Key[] keys;

    private bool canType = true;

    public Transform camPosition;

    // Start is called before the first frame update
    protected override void Start()
    {
        base.Start();

        padState = PadState.INTERACTABLE;

        ResetKeyCode();
    }

    public void AddToCode(int keyNumber)
    {
        if (!canType || screen.text.Length >= code.ToString().Length || padState == PadState.INTERACTABLE) return;

        screen.text += keyNumber.ToString();

    }

    public void ConfirmCode()
    {
        if (!canType || padState == PadState.INTERACTABLE) return;

        if (screen.text == code.ToString())
        {
            // Done Good.
            onCorrect.Invoke();
            StartCoroutine(DelayedReset("Correct.", correctColour));
        }
        else
        {
            // Done Bad.
            StartCoroutine(DelayedReset("Wrong.", incorrectColour));
        }
    }

    public void RemoveFromCode()
    {
        if (!canType || screen.text.Length <= 0 || padState == PadState.INTERACTABLE) return;

        screen.text = screen.text.Remove(screen.text.Length-1);
    }

    public void ClearCode()
    {
        if (!canType || padState == PadState.INTERACTABLE) return;

        screen.text = "";
    }

    public void ActivateKeyPad()
    {
        padState = PadState.ACTIVE;
        InputHandler.Instance.inputType = InputType.INSPECTINGKEYCODE;


        foreach (Key key in keys)
        {
            key.Activate();
        }

        SetHighlighted(false);
    }

    public void DeactivateKeyPad()
    {
        padState = PadState.INTERACTABLE;
        InputHandler.Instance.inputType = InputType.FPS;

        foreach (Key key in keys)
        {
            key.Deactivate();
        }
    }

    private IEnumerator DelayedReset(string message, Color screenTextColour)
    {
        canType = false;

        screen.text = message;
        screen.color = screenTextColour;

        yield return new WaitForSeconds(1f);
        
        ResetKeyCode();

        canType = true;
    }

    private void ResetKeyCode()
    {
        screen.text = "";
        screen.color = baseColour;
    }
}
