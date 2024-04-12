using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Item : Interactable
{
    [Header("Components")]

    public ItemSO item;

    private MeshCollider itemCollider;

    protected override void Start()
    {
        base.Start();

        itemCollider = GetComponent<MeshCollider>();

        tooltipText = item.tooltip;

        gameObject.name = item.itemName;
        mF.mesh = item.itemMesh;
        mR.material = item.itemTexture;
        if (itemCollider) itemCollider.sharedMesh = mF.mesh;
    }

    public override void OnPickup()
    {
        base.OnPickup();

        Destroy(gameObject);
    }

}
