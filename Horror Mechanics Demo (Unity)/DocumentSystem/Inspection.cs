using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Inspection : MonoBehaviour
{

    [SerializeField] private GameObject inspectionScreen;

    [SerializeField] private GameObject textOverlayGO;

    [SerializeField] private DocumentInfoReader documentInfoReader;

    private DocumentSO currentDocument;

    private void Start()
    {
        inspectionScreen.SetActive(false);

        if (textOverlayGO) textOverlayGO.SetActive(false);
    }

    public void Pickup(DocumentSO document)
    {
        if (inspectionScreen) inspectionScreen.SetActive(true);

        InputHandler.Instance.inputType = InputType.INSPECTINGDOCUMENT;

        if (textOverlayGO) textOverlayGO.SetActive(false);

        currentDocument = document;
        documentInfoReader.SetDocument(document);
    }

    public void PutDown()
    {
        if (inspectionScreen) inspectionScreen.SetActive(false);

        if (textOverlayGO) textOverlayGO.SetActive(false);

        InputHandler.Instance.inputType = InputType.FPS;
    }

    public void ToggleOverlay()
    {
        if (textOverlayGO) textOverlayGO.SetActive(!textOverlayGO.activeInHierarchy);
    }
}
