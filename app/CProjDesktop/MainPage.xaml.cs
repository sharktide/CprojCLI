using System.Diagnostics;
using System.Text.Json;

namespace CProjDesktop;

public partial class MainPage : ContentPage
{
    string appData = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
    string jsonPath => Path.Combine(appData, "cproj", "favorites.json");

    List<FavoriteItem> favorites = new();

    public MainPage()
    {
        InitializeComponent();
        LoadFavorites();
    }

    void LoadFavorites()
    {
        if (!File.Exists(jsonPath)) return;

        try
        {
            var json = File.ReadAllText(jsonPath);
            var dict = JsonSerializer.Deserialize<Dictionary<string, string>>(json);
            favorites = dict?.Select(kvp => new FavoriteItem { Id = kvp.Key, Path = kvp.Value }).ToList() ?? new();
            FavoritesList.ItemsSource = favorites;
        }
        catch (Exception ex)
        {
            Console.WriteLine("Failed to parse favorites: " + ex.Message);
        }
    }

    private async void OnAddToolbarClicked(object sender, EventArgs e)
    {
        await Navigation.PushAsync(new AddPage(LoadFavorites));
    }

    async void OnOpenClicked(object sender, EventArgs e)
    {
        if (sender is Button btn && btn.BindingContext is FavoriteItem fav)
        {
            Console.WriteLine($"Opening: {fav.Id}");
            await RunCproj($" \"{fav.Id}\"");
        }
    }
    async void OnCodeClicked(object sender, EventArgs e)
    {
        if (sender is Button btn && btn.BindingContext is FavoriteItem fav)
        {
            Trace.WriteLine($"Opening: {fav.Id}");
            await RunCproj($" -c \"{fav.Id}\"");
        }
    }


    async void OnDeleteClicked(object sender, EventArgs e)
    {
        if (sender is Button btn && btn.BindingContext is FavoriteItem fav)
        {
            await RunCproj($"-d \"{fav.Id}\"");
            LoadFavorites();
        }
    }

    async Task RunCproj(string args)
    {
        try
        {
            var psi = new ProcessStartInfo
            {
                FileName = "cproj", // or full path to your compiled CLI
                Arguments = args,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true,
            };

            var process = Process.Start(psi);
            await process.WaitForExitAsync();
        }
        catch (Exception ex)
        {
            await DisplayAlert("Error", ex.Message, "OK");
        }
    }

    public class FavoriteItem
    {
        public string Id { get; set; }
        public string Path { get; set; }
    }
}
