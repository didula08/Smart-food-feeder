import { View, Text, FlatList, StyleSheet, Dimensions } from 'react-native';
import React, { useEffect, useState } from 'react';
import { db } from '../firebase';
import { ref, onValue } from "firebase/database";

const { width } = Dimensions.get('window'); // Get screen width dynamically

export default function History() {
    const [history, setHistory] = useState([]);

    useEffect(() => {
        const historyRef = ref(db, 'history');
        onValue(historyRef, (snapshot) => {
            const data = snapshot.val();
            if (data) {
                const historyArray = Object.keys(data)
                    .map(key => ({ id: key, ...data[key] }))
                    .reverse(); // Reverse to show new data at the top
                setHistory(historyArray);
            }
        });
    }, []);

    const renderItem = ({ item }) => (
        <View style={styles.row}>
            <Text style={styles.cell} numberOfLines={1} ellipsizeMode="tail">{item.date}</Text>
            <Text style={styles.cell} numberOfLines={1} ellipsizeMode="tail">{item.time}</Text>
            <Text style={styles.cell} numberOfLines={1} ellipsizeMode="tail">{item.action}</Text>
        </View>
    );

    return (
        <View style={styles.container}>
            <Text style={styles.title}>History</Text>
            <View style={styles.tableHeader}>
                <Text style={styles.headerText}>Date</Text>
                <Text style={styles.headerText}>Time</Text>
                <Text style={styles.headerText}>Action</Text>
            </View>
            <FlatList 
                data={history} 
                keyExtractor={(item) => item.id} 
                renderItem={renderItem} 
            />
        </View>
    );
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        paddingHorizontal: width * 0.05, // 5% of screen width
        paddingVertical: 20,
        backgroundColor: '#f8f9fa',
    },
    title: {
        fontSize: width * 0.06, // Responsive font size
        fontWeight: 'bold',
        textAlign: 'center',
        marginBottom: 20,
    },
    tableHeader: {
        flexDirection: 'row',
        backgroundColor: '#0288D1',
        paddingVertical: 12,
        borderRadius: 5,
    },
    headerText: {
        fontWeight: 'bold',
        color: 'white',
        flex: 1,
        textAlign: 'center',
        fontSize: width * 0.04, // Responsive font size
    },
    row: {
        flexDirection: 'row',
        paddingVertical: 10,
        borderBottomWidth: 1,
        borderBottomColor: '#ccc',
    },
    cell: {
        flex: 1,
        textAlign: 'center',
        fontSize: width * 0.035, // Responsive font size
        paddingHorizontal: 5,
    },
});
