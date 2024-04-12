using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class DocumentInfoReader : Interactable
{

    [SerializeField] private DocumentSO document;

    [SerializeField] private RawImage gfx;
    [SerializeField] private TextMeshProUGUI easyReadText;

    // Start is called before the first frame update
    protected override void Start()
    {
        base.Start();

        if (document == null)
        {
            Debug.Log("No document selected.");

            return;
        }

        gfx.texture = document.gfx;
        easyReadText.text = document.text;
    }

    public void SetDocument(DocumentSO newDocument)
    {
        document = newDocument;

        if (document == null)
        {
            Debug.Log("No document selected.");

            return;
        }

        gfx.texture = document.gfx;
        easyReadText.text = document.text;
    }

    public DocumentSO GetDocument()
    {
        return document;
    }
}
