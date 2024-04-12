using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class InventoryLineUp : MonoBehaviour
{
    public static InventoryLineUp Instance { set; get; }

    public GameObject inventoryItem;

    [SerializeField] private TextMeshProUGUI itemNameText;
    [SerializeField] private TextMeshProUGUI itemAmountText;

    [SerializeField] private Camera cam;

    private List<Transform> itemSlots = new List<Transform>();

    private int currentItemIndex;

    [SerializeField] private float speed = 10f;
    [SerializeField] private float itemSpacing = 2f;

    [SerializeField] private float camDistance = 3f;

    [SerializeField] private TextMeshProUGUI usinText;
    [SerializeField] private GameObject usingGO;

    internal bool open = false;

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
        }
        else
        {
            Destroy(gameObject);
        }
    }

    private void Start()
    {
        ActivateUseText( false, "");
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.O))
        {
            Inventory.Instance.CloseInventory();
        }

        if (open && itemSlots.Count > 0)
        {
            CameraMovement();

            Navigation();

            if (Input.GetKeyDown(KeyCode.E))
            {
                Use();
            }
            else if (Input.GetKeyDown(KeyCode.Q))
            {
                InInventoryItem inItem = itemSlots[currentItemIndex].GetComponent<InInventoryItem>();

                Inventory.Instance.DropFromInventory(inItem.item);
                inItem.amount -= 1;

                if (inItem.amount <= 0)
                {
                    DestroyLineUp();
                    GenerateLineUp(false);
                }
            }
        }

        SetUI();
    }

    private void Use()
    {
        InInventoryItem inItem = itemSlots[currentItemIndex].GetComponent<InInventoryItem>();

        Inventory.Instance.RemoveFromInventory(inItem.item);
        inItem.amount -= 1;

        StartCoroutine(Read(inItem.item));

        if (inItem.amount <= 0)
        {
            DestroyLineUp();
            GenerateLineUp(false);
            return;
        }
    }

    private IEnumerator Read(ItemSO item)
    {
        ActivateUseText(true, "Used: " + item.itemName);
        yield return new WaitForSeconds(.4f);
        ActivateUseText(false, "");
    }

    private void CameraMovement()
    {
        Vector3 newCamPos = Vector3.MoveTowards(cam.transform.position, itemSlots[currentItemIndex].position, Time.deltaTime * speed);

        newCamPos.z = -camDistance;

        cam.transform.position = newCamPos;
    }

    public void ActivateUseText(bool state, string message)
    {
        if (usingGO.activeInHierarchy != state) usingGO.SetActive(state);
        usinText.text = message;
    }

    private void Navigation()
    {
        if (Input.GetKeyDown(KeyCode.D))
        {
            ScrollForward();
        }
        else if (Input.GetKeyDown(KeyCode.A))
        {
            ScrollBackwards();
        }
    }

    private void ScrollForward()
    {
        itemSlots[currentItemIndex].GetComponent<InInventoryItem>().SetHighlighted(false);

        currentItemIndex++;

        if (currentItemIndex >= itemSlots.Count)
        {
            currentItemIndex = 0;
        }

        itemSlots[currentItemIndex].GetComponent<InInventoryItem>().SetHighlighted(true);
    }

    private void ScrollBackwards()
    {
        itemSlots[currentItemIndex].GetComponent<InInventoryItem>().SetHighlighted(false);

        currentItemIndex--;

        if (currentItemIndex < 0)
        {
            currentItemIndex = itemSlots.Count - 1;
        }

        itemSlots[currentItemIndex].GetComponent<InInventoryItem>().SetHighlighted(true);
    }

    public void GenerateLineUp(bool resetIndexPos)
    {
        open = true;

        SubtitleHandler.Instance.ResetSubtitleReader();
                
        int i = 0;

        foreach (KeyValuePair<ItemSO, int> pair in Inventory.Instance.GetInventory())
        {
            Vector3 offset = new Vector3(itemSpacing * i, 0, 0);

            Transform newItemTransform = Instantiate(inventoryItem, transform.position + offset, Quaternion.identity).transform;

            newItemTransform.parent = gameObject.transform;

            if (newItemTransform.TryGetComponent<InInventoryItem>(out InInventoryItem item))
            {
                item.Setup(pair.Key, pair.Value);
            }

            itemSlots.Add(newItemTransform);

            i++;
        }

        if (resetIndexPos)
        {
            currentItemIndex = 0;
        }
        else if (currentItemIndex >= itemSlots.Count)
        {
            currentItemIndex = itemSlots.Count - 1;
        }

        if (itemSlots.Count > 0) itemSlots[currentItemIndex].GetComponent<InInventoryItem>().SetHighlighted(true);
    }

    public void DestroyLineUp()
    {
        open = false;

        for (int i = 0; i < itemSlots.Count; i++)
        {
            Destroy(itemSlots[i].gameObject);
        }

        itemSlots.Clear();
    }

    public void SetUI()
    {
        if (itemSlots.Count == 0)
        {
            itemNameText.text = "Empty Inventory!";
            itemAmountText.text = "";

            return;
        }

        string name = itemSlots[currentItemIndex].GetComponent<InInventoryItem>().item.itemName;
        string amount = "x" + itemSlots[currentItemIndex].GetComponent<InInventoryItem>().amount.ToString();

        if (itemNameText.text != name || itemAmountText.text != amount)
        {
            itemNameText.text = name;
            itemAmountText.text = amount;
        }
    }
}
