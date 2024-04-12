using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Inventory : MonoBehaviour
{

    [SerializeField] private Transform player;
    [SerializeField] private GameObject itemPrefab;
    public static Inventory Instance { set; get; }

    public Camera worldCam;
    public GameObject worldCanvas;

    public Camera inventoryCam;
    public GameObject inventoryCanvas;

    [SerializeField] private Dictionary<ItemSO, int> inventoryItems = new Dictionary<ItemSO, int>();

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }
    }


    private void Start()
    {
        CloseInventory();
    }

    public void AddToInventory(Item item)
    {
        if (inventoryItems.ContainsKey(item.item))
        {
            inventoryItems[item.item] += 1;
        }
        else
        {
            inventoryItems.TryAdd(item.item, 1);
        }

        item.OnPickup();
    }

    public void RemoveFromInventory(ItemSO item)
    {
        if (!inventoryItems.ContainsKey(item)) return;

        if (inventoryItems[item] == 1) inventoryItems.Remove(item);
        else inventoryItems[item] -= 1;
    }

    public void DropFromInventory(ItemSO item)
    {
        if (!inventoryItems.ContainsKey(item)) return;

        GameObject go = Instantiate(itemPrefab, player.position, Quaternion.identity);

        go.GetComponent<Item>().item = item;

        if (inventoryItems[item] == 1) inventoryItems.Remove(item);
        else inventoryItems[item] -= 1;
    }

    public void OpenInventory()
    {
        if (inventoryCam) inventoryCam.enabled = true;
        if (inventoryCanvas) inventoryCanvas.SetActive(true);

        InventoryLineUp.Instance.GenerateLineUp(true);
        InventoryLineUp.Instance.ActivateUseText(false, "");

        if (worldCam) worldCam.enabled = false;
        if (worldCanvas) worldCanvas.SetActive(false);
    }

    public void CloseInventory()
    {
        if (worldCam) worldCam.enabled = true;
        if (worldCanvas) worldCanvas.SetActive(true);

        InventoryLineUp.Instance.DestroyLineUp();
        InventoryLineUp.Instance.ActivateUseText(false, "");

        if (inventoryCam) inventoryCam.enabled = false;
        if (inventoryCanvas) inventoryCanvas.SetActive(false);
    }

    public int InventorySize()
    {
        return inventoryItems.Count;
    }

    public Dictionary<ItemSO, int> GetInventory()
    {
        return inventoryItems;
    }
}
