using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum InputType { FPS, INSPECTINGDOCUMENT, INSPECTINGKEYCODE }

[RequireComponent(typeof(Moveable))]
public class InputHandler : MonoBehaviour
{

    public static InputHandler Instance { set; get; }

    [Header("Components")]

    private Moveable moveable;

    [SerializeField] private RawImage crosshair;

    public InputType inputType = InputType.FPS;

    [SerializeField] private LayerMask interactableLayers;
    [SerializeField] private LayerMask keyPadInteractableLayers;

    [SerializeField] private float interactDistance = 1f;

    [SerializeField] private Inspection inspection;

    private Interactable interactableInView;

    internal KeyCodeReader currentCodeReader;

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
        }
        else
        {
            Destroy(this.gameObject);
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        moveable = GetComponent<Moveable>();

        if (!inspection)
        {
            inspection = GetComponent<Inspection>();
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (inputType == InputType.FPS)
        {
            if (InventoryLineUp.Instance.open) return;

            if (Cursor.lockState != CursorLockMode.Locked)
            {
                Cursor.lockState = CursorLockMode.Locked;
                Cursor.visible = false;
            }

            Vector2 moveVector = new Vector2(Input.GetAxisRaw("Horizontal"), Input.GetAxisRaw("Vertical"));

            moveable.Movement(moveVector);

            InteractFPS();

            if (Input.GetKeyDown(KeyCode.I))
            {
                Inventory.Instance.OpenInventory();
            }
        }
        else if (inputType == InputType.INSPECTINGDOCUMENT)
        {
            if (Cursor.lockState != CursorLockMode.None)
            {
                Cursor.lockState = CursorLockMode.None;
                Cursor.visible = true;
            }

            if (Input.GetKeyDown(KeyCode.O) && inspection)
            {
                inspection.PutDown();
            }
        }
        else if (inputType == InputType.INSPECTINGKEYCODE)
        {
            if (Cursor.lockState != CursorLockMode.None)
            {
                Cursor.lockState = CursorLockMode.None;
                Cursor.visible = true;
            }

            InteractKeyPad();

            if (Input.GetKeyDown(KeyCode.O) && currentCodeReader)
            {
                currentCodeReader.DeactivateKeyPad();
                currentCodeReader = null;
            }
        }

        if (crosshair)
        {
            if (inputType == InputType.FPS && !crosshair.enabled)
            {
                crosshair.enabled = true;
            }
            else if (inputType != InputType.FPS && crosshair.enabled)
            {
                crosshair.enabled = false;
            }
        }
    }

    private void InteractFPS()
    {
        SelectInFront();
        TriggerFPSInteractables();
    }

    private void InteractKeyPad()
    {
        SelectAtMouse();
        TriggerKeyPadInteractions();
    }

    private void TriggerFPSInteractables()
    {
        if (Input.GetButtonDown("Fire1") && interactableInView)
        {
            if (interactableInView.TryGetComponent<Item>(out Item item))
            {
                Inventory.Instance.AddToInventory(item);
            }
            else if (interactableInView.TryGetComponent<DocumentInfoReader>(out DocumentInfoReader document))
            {
                inspection.Pickup(document.GetDocument());
            }
            else if (interactableInView.TryGetComponent<KeyCodeReader>(out KeyCodeReader reader))
            {
                currentCodeReader = reader;
                currentCodeReader.ActivateKeyPad();
            }
            else if (interactableInView.TryGetComponent<Key>(out Key key))
            {
                key.Pressed();
            }
        }
    }

    private void SelectInFront()
    {
        if (Physics.Raycast(Camera.main.transform.position, Camera.main.transform.forward, out RaycastHit hit, interactDistance, interactableLayers))
        {
            if (hit.transform.TryGetComponent<Interactable>(out Interactable interactable))
            {
                if (interactableInView)
                {
                    if (interactable != interactableInView)
                    {
                        interactableInView.SetHighlighted(false);
                        interactableInView = null;
                    }
                }

                interactableInView = interactable;
                interactableInView.SetHighlighted(true);
            }
            else if (interactableInView)
            {
                interactableInView.SetHighlighted(false);
                interactableInView = null;
            }
        }
        else if (interactableInView)
        {
            interactableInView.SetHighlighted(false);
            interactableInView = null;
        }
    }

    private void SelectAtMouse()
    {
        if (!Camera.main) return;

        Ray mousePosRay = Camera.main.ScreenPointToRay(Input.mousePosition);

        if (Physics.Raycast(mousePosRay, out RaycastHit hit, 10, keyPadInteractableLayers))
        {
            if (hit.transform.TryGetComponent<Interactable>(out Interactable interactable))
            {
                if (interactableInView)
                {
                    if (interactable != interactableInView)
                    {
                        interactableInView.SetHighlighted(false);
                        interactableInView = null;
                    }
                }

                interactableInView = interactable;
                interactableInView.SetHighlighted(true);
            }
            else if (interactableInView)
            {
                interactableInView.SetHighlighted(false);
                interactableInView = null;
            }
        }
        else if (interactableInView)
        {
            interactableInView.SetHighlighted(false);
            interactableInView = null;
        }
    }

    private void TriggerKeyPadInteractions()
    {
        if (Input.GetButtonDown("Fire1") && interactableInView)
        {
            if (interactableInView.TryGetComponent<Key>(out Key key))
            {
                key.Pressed();
            }
        }
    }

}