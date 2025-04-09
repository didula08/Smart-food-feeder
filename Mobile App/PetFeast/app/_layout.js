import React, { useState, useEffect } from 'react';
import { View, Text, StyleSheet } from 'react-native';
import { Tabs } from 'expo-router'; // Import Tabs for navigation
import Ionicons from 'react-native-vector-icons/Ionicons';
import { useRouter } from 'expo-router';
import LottieView from 'lottie-react-native'; // Import Lottie

export default function Layout() {
  const [loading, setLoading] = useState(true);
  const router = useRouter();

  // Simulate a 2-second delay for loading screen
  useEffect(() => {
    const timer = setTimeout(() => {
      setLoading(false); // Hide loading screen after 2 seconds
    }, 3000);

    return () => clearTimeout(timer); // Cleanup on unmount
  }, []);

  if (loading) {
    return (
      <View style={styles.loadingContainer}>
        <LottieView
          source={require('../assets/Animation - 1743164532159.json')}
          autoPlay
          loop
          style={styles.loadingAnimation}
        />
        <Text style={styles.loadingText}>Loading...</Text>
      </View>
    );
  }

  // Root Layout renders the Tab Navigator or Slot for child screens
  return (
    <Tabs
      screenOptions={({ route }) => ({
        tabBarIcon: ({ focused, size }) => {
          let iconName;
          let iconColor;

          // Set icon color based on focus state
          if (focused) {
            iconColor = "#0A102B"; // Orange color for active tab
          } else {
            iconColor = "#607D8B"; // Blue-grey color for inactive tab
          }

          // Determine the icon name based on the route
          if (route.name === "index") {
            iconName = focused ? "fast-food" : "fast-food-outline";
          } else if (route.name === "schedule") {
            iconName = focused ? "calendar" : "calendar-outline";
          } else if (route.name === "history") {
            iconName = focused ? "time" : "time-outline";
          }

          return <Ionicons name={iconName} size={size} color={iconColor} />;
        },
        tabBarActiveTintColor: "#0A102B",   // Active tab color (orange)
        tabBarInactiveTintColor: "#607D8B", // Inactive tab color (blue-grey)
      })}
    >
      <Tabs.Screen name="index" options={{ title: "Manual Feed" }} />
      <Tabs.Screen name="schedule" options={{ title: "Schedule Feed" }} />
      <Tabs.Screen name="history" options={{ title: "History" }} />
    </Tabs>
  );
}

const styles = StyleSheet.create({
  loadingContainer: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#FFF',
  },
  loadingAnimation: {
    width: 300,
    height: 300,
  },
  loadingText: {
    fontSize: 18,
    color: '#333',
    marginTop: 20,
  },
});
