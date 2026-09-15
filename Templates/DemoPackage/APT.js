
APT.On(
    "Import",
    function()
    {
        APT.Set("AbytekEBT.Name", "DemoPackage");
    }
);
APT.Extensions.AbytekEBT.SetupPackage(this);