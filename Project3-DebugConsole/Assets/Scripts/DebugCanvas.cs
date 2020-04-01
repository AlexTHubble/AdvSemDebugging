using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class DebugCanvas : MonoBehaviour
{
    public bool enableDebug = true;
    [HideInInspector] public bool debugActive;
    [HideInInspector] public TMP_InputField inputField;
    TMP_Text debugText;

    TMP_Text outputLog;

    GameObject debugParent;

    GameObject obj;
    Object comp;

    System.Reflection.FieldInfo[] objInfo;

    public static DebugCanvas instance;

    Dictionary<string, string> shortcuts;

    bool select;

    Dictionary<string, string> commands;

    void Start()
    {
        shortcuts = new Dictionary<string, string>();
        commands = new Dictionary<string, string>();
        instance = this;
        debugParent = transform.GetChild(0).gameObject;
        inputField = debugParent.transform.Find("DebugInput").GetComponent<TMP_InputField>();
        debugText = debugParent.transform.Find("DebugText").GetComponent<TMP_Text>();
        outputLog = debugParent.transform.Find("OutputLog").GetComponent<TMP_Text>();
        debugParent.SetActive(false);
        commands.Add("findobject", "Finds an object by name. Uses GameObject.Find(). Usage: findobject <name>");
        commands.Add("findtype", "Finds an object by type. Uses FindObjectOfType(). Usage: findtype <type>");
        commands.Add("gettype", "Finds an object by type. Uses FindObjectOfType(). Usage: findtype <type>");
        commands.Add("getcomponent", "Gets a component of an object. Need to have an object selected first. Usage: getcomponent <componentName>");
        commands.Add("select", "Selects an object. Need to click on it after you use this command. Usage: select");
        commands.Add("setcommand", "Sets a shortcut command from an already created command. Usage: setcommand <newCommandName> <oldCommandName> <oldCommandArgs>...");
        commands.Add("setvalue", "Sets a value in a component. Need to select a component first. Usage: setvalue <varName> <newValue>");
        commands.Add("help", "Gets tips and usage for a command. Usage: help <command>");
    }

    void Update()
    {
        if((Input.GetKeyDown(KeyCode.Tilde) || Input.GetKeyDown(KeyCode.BackQuote)) && enableDebug)
        {
            debugActive = !debugActive;
            debugParent.SetActive(debugActive);
        }
        if(Input.GetMouseButtonDown(0) && select)
        {
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;

            if(Physics.Raycast(ray, out hit, 100))
            {
                obj = hit.transform.gameObject;
                comp = null;
                select = false;
            }
        }
        if(debugActive && inputField.isFocused && inputField.text != "" && Input.GetKey(KeyCode.Return))
        {
            SendCommands(inputField.text);
        }

        if (obj != null)
        {
            debugText.text = obj.name + "\n";
            if (comp != null)
            {
                debugText.text += comp.name + "\n";
                for (int i = 0; i < objInfo.Length; i++)
                {
                    debugText.text += objInfo[i].Name + ": " + objInfo[i].GetValue(comp) + "\n";
                }
            }
        }
    }

    void SendCommands(string input)
    {
        var args = input.Split();
        inputField.text = "";

        args[0] = args[0].ToLower();

        if(args[0] == "findobject")
        {
            if(args.Length <= 1)
            {
                outputLog.text = "Invalid number of arguments. " + commands["findobject"];
            }
            else
            {
                obj = GameObject.Find(args[1]);
                comp = null;
                if (obj)
                {
                    outputLog.text = "Object Found.";
                }
                else
                {
                    outputLog.text = "Object not found";
                }
            }
        }
        else if (args[0] == "findtype" || args[0] == "gettype")
        {
            if(args.Length <= 1)
            {
                outputLog.text = "Invalid number of arguments. " + commands["findtype"];
            }
            else
            {
                comp = FindObjectOfType(System.Type.GetType(args[1]));
                obj = comp.GetType().GetProperty("gameObject").GetValue(comp) as GameObject;
                objInfo = comp.GetType().GetFields(System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance);
                if (obj)
                {
                    outputLog.text = "Type found.";
                }
                else
                {
                    outputLog.text = "Type not found.";
                }
            }
        }
        else if (args[0] == "getcomponent")
        {
            if(args.Length <= 1)
            {
                outputLog.text = "Invalid number of arguments. " + commands["getcomponent"];
            }
            else
            {
                if (obj != null)
                {
                    comp = obj.GetComponent(System.Type.GetType(args[1]));
                    objInfo = comp.GetType().GetFields(System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance);
                    outputLog.text = "Component selected.";
                }
                else
                {
                    outputLog.text = "Need to select an object first with select.";
                }
            }
            
        }
        else if (args[0] == "select")
        {
            select = true;
            outputLog.text = "Now click an object to select it";
        }
        else if (args[0] == "setcommand")
        {
            if (args.Length <= 2)
            {
                outputLog.text = "Invalid number of arguments. " + commands["setcommand"];
            }
            else
            {
                string newMacro = "";
                for (int i = 2; i < args.Length; i++)
                {
                    newMacro += args[i];
                }
                shortcuts[args[1]] = newMacro;
                outputLog.text = "Command: " + args[1] + " created.";
            }
        }
        else if(args[0] == "setvalue")
        {
            if (comp != null)
            {
                if (args.Length <= 2)
                {
                    outputLog.text = "Invalid number of arguments. " + commands["setvalue"];
                }
                else if (comp)
                {
                    System.Reflection.FieldInfo info = comp.GetType().GetField(args[1], System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance);
                    if (info.FieldType.ToString() == "System.String")
                    {
                        info.SetValue(comp as object, args[2]);
                    }
                    else if (info.FieldType.ToString() == "System.Int32")
                    {
                        info.SetValue(comp, int.Parse(args[2]));
                    }
                    else if (info.FieldType.ToString() == "System.Single")
                    {
                        info.SetValue(comp, float.Parse(args[2]));
                    }
                    else if (info.FieldType.ToString() == "UnityEngine.Vector3")
                    {
                        if (args.Length <= 4) outputLog.text = "Invalid number of arguments";
                        else info.SetValue(comp, new Vector3(float.Parse(args[2]), float.Parse(args[3]), float.Parse(args[4])));
                    }
                    else
                    {
                        outputLog.text = "Unknown type: " + info.FieldType.ToString() + ". Please contact the developer.";
                    }
                }
            }
            else
            {
                outputLog.text = "Need to set a component first";
            }
        }
        else if(args[0] == "help")
        {
            if (args.Length <= 1)
            {
                outputLog.text = "Invalid number of arguments. " + commands["help"];
            }
            else
            {
                if(commands.ContainsKey(args[1]))
                {
                    outputLog.text = commands[args[1]];
                }
                else
                {
                    outputLog.text = "No command found for: " + args[1];
                }
            }
        }
        //Shortcuts
        else if (shortcuts.ContainsKey(args[0]))
        {
            SendCommands(shortcuts[args[0]]);
        }
        //Default
        else
        {
            outputLog.text = "Unknown command: " + input;
        }
    }
}
