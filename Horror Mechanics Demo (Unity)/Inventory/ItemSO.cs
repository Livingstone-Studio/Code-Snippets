using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;


[CreateAssetMenu(menuName = "My Assets/Item ")]
public class ItemSO : ScriptableObject
{
    public string itemName = "Item";
    public Mesh itemMesh;
    public Material itemTexture;
    public string tooltip = "Pickup";

}
