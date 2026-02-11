//
// Created by Michal Přikryl on 11.02.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
using System.Buffers;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;
using System.Numerics;
using System.Text;
using System.Text.Json;
using System.Text.RegularExpressions;
using CounterStrikeSharp.API;
using CounterStrikeSharp.API.Core;
using CounterStrikeSharp.API.Core.Attributes.Registration;
using CounterStrikeSharp.API.Modules.Commands;
using CounterStrikeSharp.API.Modules.Memory;
using CounterStrikeSharp.API.Modules.Utils;
using Vector = CounterStrikeSharp.API.Modules.Utils.Vector;

namespace RayTraceImpl;

// ReSharper disable once InconsistentNaming
// ReSharper disable once UnusedType.Global
public class RayTraceImpl : BasePlugin
{
    public override string ModuleName => "RayTraceImpl";
    public override string ModuleVersion => m_pszVersion;
    public override string ModuleAuthor => "Slynx";

    public override void Load(bool hotReload)
    {
        RegisterListener<Listeners.OnMetamodAllPluginsLoaded>(OnMetamodAllPluginsLoaded);
    }

    public override void Unload(bool hotReload)
    {
        RemoveListener<Listeners.OnMetamodAllPluginsLoaded>(OnMetamodAllPluginsLoaded);
    }

    private void OnMetamodAllPluginsLoaded()
    {

    }

    public static string m_pszVersion { get; private set; } = "";
}