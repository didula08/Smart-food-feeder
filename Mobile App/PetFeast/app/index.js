// import React, { useState, useEffect } from "react";
// import { ScrollView, Text, TouchableOpacity, View, StyleSheet, Dimensions } from "react-native";
// import { Card } from "react-native-paper";
// import Ionicons from "react-native-vector-icons/Ionicons";
// import { db, set, push } from "../firebase";
// import { ref, onValue } from "firebase/database";

// const { width } = Dimensions.get('window');

// export default function HomeScreen() {
//   const [waterCount, setWaterCount] = useState(0);
//   const [foodCount, setFoodCount] = useState(0);

//   useEffect(() => {
//     fetchTodayHistory();
//   }, []);

//   const fetchTodayHistory = () => {
//     const historyRef = ref(db, "history");
//     const today = new Date().toLocaleDateString();

//     onValue(historyRef, (snapshot) => {
//       const data = snapshot.val();
//       if (data) {
//         const historyArray = Object.values(data);
//         const todayHistory = historyArray.filter((item) => item.date === today);

//         setWaterCount(todayHistory.filter((item) => item.action === "Water").length);
//         setFoodCount(todayHistory.filter((item) => item.action === "Food").length);
//       }
//     });
//   };

//   const handleFeedWater = () => {
//      set(ref(db, `watercount`), true)
//       .then(() => {
//         alert(`Water Supplied!`);
//         addToHistory('Water');

//         setTimeout(() => {
//           set(ref(db, `watercount`), false);
//         }, 10000);
//       })
//       .catch((error) => console.error(`Error setting water supply:`, error));
//   };
//   const handleFeedFood = () => {
//     set(ref(db, `foodcount`), true)
//      .then(() => {
//        alert(`Food Supplied!`);
//        addToHistory('Food');

//        setTimeout(() => {
//          set(ref(db, `foodcount`), false);
//        }, 10000);
//      })
//      .catch((error) => console.error(`Error setting food supply:`, error));
//  };

//   const addToHistory = (action) => {
//     const currentDate = new Date();
//     const historyRef = ref(db, "history");

//     push(historyRef, {
//       date: currentDate.toLocaleDateString(),
//       time: currentDate.toLocaleTimeString(),
//       action,
//     }).then(fetchTodayHistory)
//       .catch((error) => console.error("Error updating history:", error));
//   };

//   return (
//     <ScrollView style={styles.container}>
//       <Text style={styles.mainTitle}>Today's Summary</Text>

//       <View style={styles.summaryContainer}>
//         <Card style={[styles.card, { borderLeftColor: "#0288D1" }]}> 
//           <Card.Content>
//             <View style={styles.summaryItem}>
//               <Ionicons name="water" size={50} color="#0288D1" />
//               <Text style={styles.summaryText}>Water Supplied</Text>
//               <Text style={styles.valueText}>{waterCount} times</Text>
//             </View>
//           </Card.Content>
//         </Card>

//         <Card style={[styles.card, { borderLeftColor: "#FF5722" }]}> 
//           <Card.Content>
//             <View style={styles.summaryItem}>
//               <Ionicons name="fast-food" size={50} color="#FF5722" />
//               <Text style={styles.summaryText}>Food Supplied</Text>
//               <Text style={styles.valueText}>{foodCount} times</Text>
//             </View>
//           </Card.Content>
//         </Card>
//       </View>

//       <View style={styles.buttonContainer}>
//         <TouchableOpacity style={[styles.button, { backgroundColor: "#0288D1" }]} onPress={() => handleFeedWater()}>
//           <Ionicons name="water" size={24} color="#fff" />
//           <Text style={styles.buttonText}>Supply Water</Text>
//         </TouchableOpacity>

//         <TouchableOpacity style={[styles.button, { backgroundColor: "#FF5722" }]} onPress={() => handleFeedFood()}>
//           <Ionicons name="fast-food" size={24} color="#fff" />
//           <Text style={styles.buttonText}>Supply Food</Text>
//         </TouchableOpacity>
//       </View>
//     </ScrollView>
//   );
// }

// const styles = StyleSheet.create({
//   container: {
//     flex: 1,
//     paddingHorizontal: width * 0.05,
//     paddingTop: 20,
//   },
//   mainTitle: {
//     fontSize: width * 0.06,
//     fontWeight: 'bold',
//     marginBottom: 20,
//     textAlign: 'center',
//     color: '#333',
//   },
//   summaryContainer: {
//     flexDirection: 'row',
//     justifyContent: 'space-between',
//     marginBottom: 10,
//   },
//   card: {
//     width: '45%',
//     padding: 15,
//     borderRadius: 10,
//     backgroundColor: '#ffffff',
//     elevation: 5,
//     borderLeftWidth: 5,
//     shadowColor: '#000',
//     shadowOpacity: 0.5,
//     shadowOffset: { width: 5, height: 5 },
//     shadowRadius: 10,
//     alignItems: 'center',
//   },
//   summaryItem: {
//     alignItems: 'center',
//   },
//   summaryText: {
//     fontSize: width * 0.045,
//     marginTop: 10,
//     fontWeight: 'bold',
//     color: '#333',
//     textAlign: 'center',
//   },
//   valueText: {
//     fontSize: width * 0.04,
//     color: '#666',
//     marginTop: 5,
//   },
//   buttonContainer: {
//     alignItems: 'center',
//     marginTop: 20,
//   },
//   button: {
//     flexDirection: 'row',
//     alignItems: 'center',
//     paddingVertical: 12,
//     paddingHorizontal: 20,
//     borderRadius: 10,
//     marginVertical: 10,
//     width: '80%',
//     justifyContent: 'center',
//   },
//   buttonText: {
//     color: '#fff',
//     fontSize: width * 0.035,
//     fontWeight: 'bold',
//     marginLeft: 10,
//   },
// });



import React, { useState, useEffect } from "react";
import { ScrollView, Text, TouchableOpacity, View, StyleSheet, Dimensions } from "react-native";
import { Card } from "react-native-paper";
import Ionicons from "react-native-vector-icons/Ionicons";
import { db, set, push } from "../firebase";
import { ref, onValue } from "firebase/database";

const { width } = Dimensions.get('window');

export default function HomeScreen() {
  const [waterCount, setWaterCount] = useState(0);
  const [foodCount, setFoodCount] = useState(0);

  useEffect(() => {
    fetchTodayHistory();
  }, []);

  const fetchTodayHistory = () => {
    const historyRef = ref(db, "history");
    const today = new Date().toLocaleDateString();

    onValue(historyRef, (snapshot) => {
      const data = snapshot.val();
      if (data) {
        const historyArray = Object.values(data);
        const todayHistory = historyArray.filter((item) => item.date === today);

        setWaterCount(todayHistory.filter((item) => item.action === "Water").length);
        setFoodCount(todayHistory.filter((item) => item.action === "Food").length);
      }
    });
  };

  const handleFeedWater = () => {
    // 🔁 Updated path to "SmartFeeder/watercount"
    set(ref(db, `SmartFeeder/watercount`), true)
      .then(() => {
        alert(`Water Supplied!`);
        addToHistory('Water');

        setTimeout(() => {
          // 🔁 Updated path to "SmartFeeder/watercount"
          set(ref(db, `SmartFeeder/watercount`), false);
        }, 10000);
      })
      .catch((error) => console.error(`Error setting water supply:`, error));
  };

  const handleFeedFood = () => {
    // 🔁 Updated path to "SmartFeeder/foodcount"
    set(ref(db, `SmartFeeder/foodcount`), true)
      .then(() => {
        alert(`Food Supplied!`);
        addToHistory('Food');

        setTimeout(() => {
          // 🔁 Updated path to "SmartFeeder/foodcount"
          set(ref(db, `SmartFeeder/foodcount`), false);
        }, 10000);
      })
      .catch((error) => console.error(`Error setting food supply:`, error));
  };

  const addToHistory = (action) => {
    const currentDate = new Date();
    const historyRef = ref(db, "history");

    push(historyRef, {
      date: currentDate.toLocaleDateString(),
      time: currentDate.toLocaleTimeString(),
      action,
    }).then(fetchTodayHistory)
      .catch((error) => console.error("Error updating history:", error));
  };

  return (
    <ScrollView style={styles.container}>
      <Text style={styles.mainTitle}>Today's Summary</Text>

      <View style={styles.summaryContainer}>
        <Card style={[styles.card, { borderLeftColor: "#0288D1" }]}>
          <Card.Content>
            <View style={styles.summaryItem}>
              <Ionicons name="water" size={50} color="#0288D1" />
              <Text style={styles.summaryText}>Water Supplied</Text>
              <Text style={styles.valueText}>{waterCount} times</Text>
            </View>
          </Card.Content>
        </Card>

        <Card style={[styles.card, { borderLeftColor: "#FF5722" }]}>
          <Card.Content>
            <View style={styles.summaryItem}>
              <Ionicons name="fast-food" size={50} color="#FF5722" />
              <Text style={styles.summaryText}>Food Supplied</Text>
              <Text style={styles.valueText}>{foodCount} times</Text>
            </View>
          </Card.Content>
        </Card>
      </View>

      <View style={styles.buttonContainer}>
        <TouchableOpacity style={[styles.button, { backgroundColor: "#0288D1" }]} onPress={handleFeedWater}>
          <Ionicons name="water" size={24} color="#fff" />
          <Text style={styles.buttonText}>Supply Water</Text>
        </TouchableOpacity>

        <TouchableOpacity style={[styles.button, { backgroundColor: "#FF5722" }]} onPress={handleFeedFood}>
          <Ionicons name="fast-food" size={24} color="#fff" />
          <Text style={styles.buttonText}>Supply Food</Text>
        </TouchableOpacity>
      </View>
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    paddingHorizontal: width * 0.05,
    paddingTop: 20,
  },
  mainTitle: {
    fontSize: width * 0.06,
    fontWeight: 'bold',
    marginBottom: 20,
    textAlign: 'center',
    color: '#333',
  },
  summaryContainer: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    marginBottom: 10,
  },
  card: {
    width: '45%',
    padding: 15,
    borderRadius: 10,
    backgroundColor: '#ffffff',
    elevation: 5,
    borderLeftWidth: 5,
    shadowColor: '#000',
    shadowOpacity: 0.5,
    shadowOffset: { width: 5, height: 5 },
    shadowRadius: 10,
    alignItems: 'center',
  },
  summaryItem: {
    alignItems: 'center',
  },
  summaryText: {
    fontSize: width * 0.045,
    marginTop: 10,
    fontWeight: 'bold',
    color: '#333',
    textAlign: 'center',
  },
  valueText: {
    fontSize: width * 0.04,
    color: '#666',
    marginTop: 5,
  },
  buttonContainer: {
    alignItems: 'center',
    marginTop: 20,
  },
  button: {
    flexDirection: 'row',
    alignItems: 'center',
    paddingVertical: 12,
    paddingHorizontal: 20,
    borderRadius: 10,
    marginVertical: 10,
    width: '80%',
    justifyContent: 'center',
  },
  buttonText: {
    color: '#fff',
    fontSize: width * 0.035,
    fontWeight: 'bold',
    marginLeft: 10,
  },
});
