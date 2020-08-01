using System;
using System.Collections.Generic;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms;
using System.IO;
using System.Security.RightsManagement;

namespace csLauncher
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        string filePathOBJ = "";
        string fileNameOBJ = "";

        uint resWidth = 1024;
        uint resHeight = 576;

        
        public MainWindow()
        {
            InitializeComponent();
        }

        private void glPathTracerStart(object sender, RoutedEventArgs e)
        {
            ProcessStartInfo startInfo = new ProcessStartInfo("glPathTracer.exe");
            startInfo.WindowStyle = ProcessWindowStyle.Normal;
            Process.Start(startInfo);
        }

        private void LoadOBJ(object sender, RoutedEventArgs e)
        {
            FrameworkElement element = new FrameworkElement();
            element.GetType().GetProperty("fLabel");

            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Title = "open OBJ File";
                openFileDialog.Multiselect = false;
                openFileDialog.InitialDirectory = Directory.GetCurrentDirectory();
                openFileDialog.Filter = "obj files (*.obj)|*.obj";
                openFileDialog.FilterIndex = 2;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    //Get the path of specified file
                    fileNameOBJ = openFileDialog.SafeFileName;
                    filePathOBJ = openFileDialog.FileName;

                    //fileLabel.Content = fileNameOBJ;
                }
            }

        }

    }

}
