using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace csLauncher
{
    public class glPathTracerLauncher
    {
        //public variables
        public string filePathOBJ = "";
        public string fileNameOBJ = "";

        public uint resWidth = 1280;
        public uint resHeight = 720;

        //private variables
        private ProcessStartInfo _startInfo;
        private string _applicationName;
        private string _appKey;

        //functions
        public glPathTracerLauncher()
        {
            _appKey = "6OWNObpW4iBrqNnA8r2L"; //key for launching the application (-> compare cliHandler.h)
            _applicationName = "glPathTracer.exe";
            _startInfo = new ProcessStartInfo(_applicationName);
            _startInfo.WindowStyle = ProcessWindowStyle.Normal;
        }

        public void LaunchRenderEngine()
        {
            //pass command line arguments with the execution invocation
            _startInfo.Arguments = _appKey + " " + resWidth + " " + resHeight + " " + filePathOBJ + " " + fileNameOBJ;
            Process.Start(_startInfo);
        }
    }
}
