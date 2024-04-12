using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InInventoryItem : MonoBehaviour
{
    [Header("Components")]

    internal MeshFilter mF;
    internal MeshRenderer mR;

    public ItemSO item;

    public int amount;

    [SerializeField] private Vector3 selectedScale;
    private Vector3 baseScale;

    private bool selected = false;


    [SerializeField] private float rotateSpeed = 20f;
    private Vector3 baseRotation;

    private void Start()
    {
        baseScale = transform.localScale;
        baseRotation = transform.localEulerAngles;
    }

    private void Update()
    {
        DynamicScaling();
        DynamicRotation();
    }

    private void DynamicRotation()
    {
        if (selected)
        {
            transform.localEulerAngles = new Vector3(transform.localEulerAngles.x, transform.localEulerAngles.y + (rotateSpeed * Time.deltaTime), transform.localEulerAngles.z);
        }
        else if (!selected && transform.localEulerAngles != baseRotation)
        {
            transform.localEulerAngles = baseRotation;
        }
    }

    private void DynamicScaling()
    {
        if (transform.localScale != selectedScale && selected)
        {
            transform.localScale = selectedScale;
        }
        else if (transform.localScale != baseScale && !selected)
        {
            transform.localScale = baseScale;
        }
    }

    // Start is called before the first frame update
    public void Setup(ItemSO newitem, int newAmount)
    {
        mF = GetComponent<MeshFilter>();
        mR = GetComponent<MeshRenderer>();

        item = newitem;
        amount = newAmount;

        SetHighlighted(false);

        gameObject.name = item.itemName;
        mF.mesh = item.itemMesh;
        mR.material = item.itemTexture;
    }

    public void SetHighlighted(bool state)
    {
        selected = state;
    }
}
