pipeline {
  options {
        disableConcurrentBuilds()
    }
  agent {
    node {
      label 'Windows'
      customWorkspace 'SteamBridge/SteamBridge'
    }

  }
  stages {
    stage('Build') {
      steps {
        bat 'C:\\build-scripts/SteamBridge/build.bat'
      }
    }

  }
}