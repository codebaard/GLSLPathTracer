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
using System.Threading;
using System.Security.RightsManagement;

namespace csLauncher
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private glPathTracerLauncher _launcher;
        private bool _resolutionSelected = false;
        private bool _fileSelected = false;
        //private System.Windows.Forms.Label fLabel;

        public MainWindow()
        {
            InitializeComponent();
            _launcher = new glPathTracerLauncher();
        }

        private void glPathTracerStart(object sender, RoutedEventArgs e)
        {
            if(_resolutionSelected && _fileSelected)
                _launcher.LaunchRenderEngine();
            else
                System.Windows.MessageBox.Show("Please select a file and set desired resolution.", "Launcher", MessageBoxButton.OK, MessageBoxImage.Error);
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
                    _launcher.fileNameOBJ = openFileDialog.SafeFileName;
                    _launcher.filePathOBJ = openFileDialog.FileName;

                    Dispatcher.BeginInvoke(new Action(() =>
                    {
                        this.fLabel.Content = openFileDialog.SafeFileName;
                    }));
                    Thread.Sleep(1);
                }
            }

            _fileSelected = true;

        }

        private void Btn1_Checked(object sender, RoutedEventArgs e)
        {

            _launcher.resWidth = 1280;
            _launcher.resHeight = 720;
            _resolutionSelected = true;
        }

        private void Btn2_Checked(object sender, RoutedEventArgs e)
        {
            //IsChecked="True" 
            _launcher.resWidth = 1024;
            _launcher.resHeight = 576;
            _resolutionSelected = true;
        }

        private void Btn3_Checked(object sender, RoutedEventArgs e)
        {
            _launcher.resWidth = 800;
            _launcher.resHeight = 450;
            _resolutionSelected = true;
        }

    }

}
