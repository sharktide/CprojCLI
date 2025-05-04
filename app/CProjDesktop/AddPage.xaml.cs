using System.Diagnostics;

namespace CProjDesktop;

public partial class AddPage : ContentPage
{
    private readonly Action onAdded;

    public AddPage(Action onAdded)
    {
        InitializeComponent();
        this.onAdded = onAdded;
    }

    private async void OnAddClicked(object sender, EventArgs e)
    {
        if (string.IsNullOrWhiteSpace(IdEntry.Text) || string.IsNullOrWhiteSpace(PathEntry.Text))
            return;

        var id = IdEntry.Text;
        var path = PathEntry.Text;

        var psi = new ProcessStartInfo
        {
            FileName = "cproj",
            Arguments = $"-a \"{path}\" \"{id}\"",
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
            CreateNoWindow = true,
        };

        try
        {
            var process = Process.Start(psi);
            await process.WaitForExitAsync();
            onAdded?.Invoke();
            await Navigation.PopAsync(); // go back to main page
        }
        catch (Exception ex)
        {
            await DisplayAlert("Error", ex.Message, "OK");
        }
    }
}
